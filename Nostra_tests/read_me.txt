	How work this application:
	Previously, put this application in nostra root directory
	First step:     creating application folder 'nost_bin/' in current directory. Make sure you have administrator access
	Second step:    reading file 'ip.dat'. This file content names of tests, that being calculate. Names precisely links to folders with input data. Assumed, that 'TN' - first test in tests list
	Third step:     creating directory 'nost_bin/results/TN/ that will be store results files
	There is starts a cycle: 
	Fourth step:    opening directory 'nost_bin/tests/TN', that contains test with name TN.
	Fifth step:	 reading file 'HINSTANCE.vmn' with list of file names, that will be copied in current nostra work directory
	Sixth step:	 begining of calculations - run NOSTRA.exe
	Seventh step:   copying results files to 'nost_bin/results/', switch TN to TN+1, and back to step 3.
