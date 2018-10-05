UDP Sender&Receiver
===================

This pair of examples shows how to encode and transmit some random data over
a UDP socket, then receive and decode that data at the other end.
The sender and receiver applications are standalone, and you can run them on
the same computer or over an actual IP network.

Note that the receiver should be started **before** the sender!

.. contents:: Table of Contents
   :local:

The sender application
----------------------

The example code for the sender is shown below.

.. literalinclude:: /../examples/udp_sender_receiver/udp_sender.c
    :language: c
    :linenos:

The receiver application
------------------------

The example code for the receiver is shown below.

.. literalinclude:: /../examples/udp_sender_receiver/udp_receiver.c
    :language: c
    :linenos:
