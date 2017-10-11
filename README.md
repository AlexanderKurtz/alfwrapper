# alfwrapper - Application-level firewalling using systemd socket action and eBPF filters

Thanks to systemd, there is now a widely supported
[protocol](https://www.freedesktop.org/software/systemd/man/sd_listen_fds.html#)
for passing ready-to-use sockets to system daemons without having to spawn a new
instance of the daemon for each connection as with ```inetd```. Furthermore,
thanks to a long-term effort by various people, we now have a [general purpose
VM](https://www.kernel.org/doc/Documentation/networking/filter.txt) in the Linux
kernel which can, amongst other things, be used to attach a filter to a socket
which decides which packets are forwarded to the application and which aren't.
Finally, thanks to the [LLVM
developers](https://github.com/llvm-mirror/llvm/tree/master/lib/Target/BPF) and
the [BCC Project](https://github.com/iovisor/bcc) we have a way to program these
filters in a restricted form of C and also a nice API to easily handle and
attach them.

With these three components in place (socket protocol, universal VM, and C API)
we can now create a program which creates a socket, attaches a filter to it, and
then passes the ready-to-use (but filtered) socket to the main program. This is
precisely what ```alfwrapper``` does.

## Why?

While you can use classic system-level-firewalling or specific bind(2)
parameters to limit which packets can reach your application, there are several
use cases which neither of these options can cover:

  1. As an **application developer** you'll typically want to bind(2) on the
     wildcard address (```::``` or ```0.0.0.0```) because these are always
     guaranteed to be available and therefore allow your application to start
     successfully [regardless of whether its ordered properly in the system
     startup
     sequence](https://www.freedesktop.org/wiki/Software/systemd/NetworkTarget/).
     However, what if you really only want to listen to a specific IP addresses
     or even have a complex rule which packets should be able to reach your
     application? ```alfwrapper``` allows you to ship an eBPF filter along with
     your application which will restrict the socket according to your specific
     requirements before your application is even started.
  2. As a **regular user** you may want to restrict the exposure of certain
     services to the network. Unfortunately configuring the system firewall
     usually requires root priviledges and is a complicated and error-prone
     task. With ```alfwrapper``` you can firewall your application according to
     arbitrary rules without any priviledges and without having to care about
     the global, system-level firewall and its limitation.
  3. As a **network tool developer** you can implement complex firewalling tools
     on top of ```alfwrapper```. Things like a port-knocking-solution or traffic
     analysis should be relatively easy once the rough edges of ```alfwrapper```
     have been removed.

## Cut the crap, what can it actually do NOW?

For example, this:

	$ ./alfwrapper --address :: --port 4242 ebpf/whitelist-private.c scripts/echod.sh 
	Received 1 descriptors via the environment.
	Listening on [::]:4242 as 3.
	...

This started a simple echo daemon on port 4242. Despite the fact that its socket
is bound to the wildcard IPv6 address, it will only accept connections from
private or loopback IPv4/IPv6 address (see the ```ebpf/whitelist-private.c```
file for details).

## Ok, but so I have to write my own filter everytime?

No. While we think that the main usage of ```alfwrapper``` will be applying
custom and application-specific filters to sockets, we do support general
purpose filters which can be customized on the command line:

	$ ./alfwrapper --address :: --port 4242 --set allow4,uint32_t:0,subnet4:127.0.0.1/24 --set allow6,uint32_t:0,subnet6:::1/128 ebpf/whitelist.c scripts/echod.sh
	Received 1 descriptors via the environment.
	Listening on [::]:4242 as 3.
	...

The ```ebpf/whitelist.c``` filter provides two maps called ```allow4``` and
```allow6``` to which the above command dynamically adds the subnets
127.0.0.1/24 and ::1/128. You can of course add more and/or different subnets.

## This seems cool. But I bet it's slow as hell right?

No, not really:

```
$ scripts/speedtest.sh 

Spawning a simple echo daemon on [::1]:4242 *without* aflwrapper

Reading 1GB of data from /dev/zero through the echo daemon
0+122046 records in
0+122046 records out
1000000000 bytes (1.0 GB, 954 MiB) copied, 2.02915 s, 493 MB/s

Spawning a simple echo daemon on [::1]:4242 *with* aflwrapper with lots of rules

Reading 1GB of data from /dev/zero through the echo daemon
0+121980 records in
0+121980 records out
1000000000 bytes (1.0 GB, 954 MiB) copied, 2.10963 s, 474 MB/s
```
