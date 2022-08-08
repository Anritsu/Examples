# Anritsu SCPI Commands example solutions

This repository contains examples for commanding Anritsu devices through SCPI commands.\
More detailed information is provided in the Readme files present in each solution's containing folder.

>At this moment, we have examples available for the following programming languages/environments:  
>   
>- C++  
>- C#  
>- MATLAB  
>- Python
  
### For general-purpose use of any Anritsu device via SCPI commands please see the contents of the _"GenericExamples"_ folder.
The examples there show simple usages of _**write**_ and _**query**_ commands for your device, and, where it's needed, also have a definition of a helpful class that handles some aspects of the communication (such as _arbitrary block data_ reading) to simplify the work of the user.

### For specific help with VNA devices please see the contents of the _"BenchtopVNA's"_ folder.
The examples there, in addition to using the helpful class definitions described above, also contain a more rigorous configuration of the VNA.

The examples show how to set up a sweep, how to configure traces, how to perform a calibration, and how to read the device data after performing a sweep.
<br><br><br>  
For any questions or inquiries regarding the present examples, please contact us at [github@anritsu.com](mailto:github@anritsu.com)