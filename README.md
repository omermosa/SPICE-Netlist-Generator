# SPICE-Netlist-Generator
#### This project was a part of Fundamental Microelectronics Course at the American University in Cairo (AUC)

The program is generating the data part of the SPICE netlist of a given Boolean Expressions 
with NO spaces in between. The input expression must NOT include any parenthesis, spaces, or 
feedback (i.e. the output cannot be included in the equation of the input. E.g. we don’t allow inputs in the form y=a&amp;y|c). 
In addition, the inputs are case sensitive.
The program respects the guidelines and the assumptions outlined in the project description.
The output statements describing the transistors are in the format: 
