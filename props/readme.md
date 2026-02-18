Here are the properties files I use in my visual studio projects.
These files are loaded from the root of my source tree where I store all my dev projetcs.

For my Visual STUDIO projects I set an environment variable OPENSSL that points to the base
of my OpenSSL installation. This is an old Open SSL that I'm still using for hash and 
encryption because this OpenSSL runs in Windows XP.

The programm that are using SSL for the web are using my newer Open SSL 3.6 because the Old
ssl version does not work with newer SSL Servers.

The newer Open SSLs are installed in /$(OPENSSL)/WIN32 and /$(OPENSSL)/X64

I have changed the Open SSL path ../lib/$(PLATFORM)/.. because the plattform is
allready encoded in the base. Therefore I moved all library files one level up.
