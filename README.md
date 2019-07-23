PeeknPoke
=========

Overview
--------

PeeknPoke tool is an integrated debugging tool to basically peek and poke the registers of different subsystems. The goal of this project is to present a simple command line interface to the user without any dependencies on external libraries, Linux distribution version, and platform.


Design principles
-----------------

When submitting any new changes to PeeknPoke keep in mind the following design principles. The rules are simple

    Platform portability: The code should be generic without any dependecy on the Platform. Avoid hard coding the register addresses as this would break the portability.
    Linux Portability: The code shouldn't include any Android or any other third party libraries. Nor should it depend on any custom Kernel modules.

Following features are supported

    Read/Write to IO memory mapped address. Dump range of memory addresses.
    MSIC/PMIC registers read and write.
    MSR registers read and write
    I2C registers read and write
    North complex registers read and write

Dependencies
------------

The users should note that there are other discrete tools in opensource that provides features such as
lm-sensors (http://www.lm-sensors.org/wiki/I2CTools) for accesing the I2c registers. MSR tools to access
CPU MSR registers (http://rpmfind.net/linux/rpm2html/search.php?query=msr-tools%28x86-32%29). PeeknPoke
tool consolidates similar functionalities in one tool so that the users don't have to worry installing
multiple tools while debugging the platform.


Contributors
------------
Commit message was not upstreamed due to there have confidential information in this project.
Confidential information was removed and upstreamed as a whole project.

list the contributors for the previous commits:
Hari Kanigeri
Asutosh Pathak
Christophe Guerard
Chandra Sekhar Anagani
Sylvain Chouleur
Imberton, Guilhem
Ben Duckworth
Guilhem IMBERTON
Clement Calmels
Haithem Salah
Vasile Iamandi
Benjamin Pujol
Mougaida, Aymen DhiaX
Jeremy Rocher
Selma BENSAID
