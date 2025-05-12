# CheckForDebugger
Checking if the current process is being debugged and shows an example of how an attacker can manipulate the flow of the program to avoid being reverse engineered. This is a uncommon technique using the juiciest part of user land, of course the PEB.


Also, this will work for a remote process you just need to get the pi or "Process Information" and use that to set the handle to the remote process using OpenProcess().

Try to debug this program with a debugger and see the outcome. HINT: No fake exec

Run this program normally and it will "run" a fake exection model. (its just a string nothing is executed read the code)

