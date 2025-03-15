1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client know that a message is fully received when it gets the EOT ascaii code. To handle potential partial read, we can just continue to read from the socket until the EOT code is sent, so if a message doesnt come all at once it will continue reading until the server says its done.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A good way to go about this is to send a message through the network that has both a header and a payload. The header for the message should be a specific size, as well as, hold the length of the payload and should be a standard on both sides (send and recieve). This allows for the recieving end to read the header and determine where the message starts and ends. However, challenges can arise if the structure of the header is not standarized, such that the recieving end cannot read the message accurately.

3. Describe the general differences between stateful and stateless protocols.

TCP is an example of a statefull protocol, in which a stream of bytes is continues and the server will keep track of the current state of the connection, refusing to continue if that connection is lost. UDP on the other hand is stateless, in which each "package" it sends are independent of each other, meaning if the connection goes down between sends that new data is lost as it doesnt know the connection is gone.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

You would use UDP if you need to send data quickly and dont necessarly care if you lose some. An example would be a temperature sensor, in which it sends data every minute. If some data gets lost from time to time its not a big deal since the temperature isnt going to change drastically in one minute.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The interface for applications to use the network is the socket syscall, along with underlying read/write syscall for send and recv. The OS sort of abstracts this by just treating socket/network communication as any other file descriptor in which a program could interact with.
