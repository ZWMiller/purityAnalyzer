#Purity Analyzer

This code is based on the StElectronPurityMaker class I wrote. It takes the
output N-dimensional histograms and extracts the nSigmaE (dE/dx) spectra,
converting it into many 1D histograms for various pT and eta regions (and
centrality). The N-dimensional histograms are necessary to save space... they
esntialyl maintain a tuple structure where I can see the correlations between
each tracks nSigmaE value and it's kinematic values... but it bins the data so
that I don't need to save the value for each track, instead I can just save
bulk correlations in the N-dimensionally binned space. 

Unforunately, this code is based on my much older analysis code before I
learned to take maximum advantage of classes, object arrays and the like....
so it's quite an ugly bit of code with lots of repetition. But it does work
well, so I haven't yet dug into refactoring.
