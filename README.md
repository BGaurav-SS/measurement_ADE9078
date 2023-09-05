# ade9078_Measurement
This repo includes the code for measuring electrical parameters with ADE9078.

----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------
                                    Mapping the power register reading to power usage (Watts).
	
The IC allows full-scale voltage of 0.707Vrms in its pins. So, the maximum full-scale voltage is 0.707V. The external circuit        should make sure that the voltage is within this limit.

In case of ADE9078 based evaluation-board, the external circuit consists of voltage divider where voltage is divided as follows: 
Voltage at IC’s pin (Vpin)= Input voltage (Vi)×1000/(1000 + 1,000,000)
The testing was carried out with Input Voltage = 100Vrms. So, Vpin = 0.1
Full-scale pin voltage of 0.707V corresponds to 708V input voltage.

The CT transformer currently used is CTT-10-CLS-CV25. It specifies 5V output for 25A input. Since the IC allows only 0.707Vrms, the maximum input is calculated to be 3.535A rms.

So, the full-scale power is 708V * 3.535A = 2502W. The register reads full scale power as 20823646. 
20823646/2502 = 8330 reading per watt.

To convert register reading into watt, divide by 8330.
Watts = [Register Reading]/8330.

The ADE9078 has an example in quick start section. That example’s result could not be used because the CT transformer we are using has different specifications. 

----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------