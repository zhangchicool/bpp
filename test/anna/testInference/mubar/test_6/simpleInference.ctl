seed =  2
*seed =  -1

seqfile = simulate.txt
*treefile=simulate_trees.txt
Imapfile = simple.Imap.txt
outfile = out.txt
mcmcfile = mcmc.txt
datefile = realTimeSeqDates.txt

# fixed number of species/populations 
speciesdelimitation = 0

# fixed species tree


species&tree = 4  A B C D
                  5  4 6 5
		  ((A, B), (C, D));

# phased data for population
phase =   0 0 0 0

# use sequence likelihood
usedata = 1

*nloci = 1
nloci = 10
clock = 1
*model = 0

# invgamma(a, b) for root tau & Dirichlet(a) for other tau's
tauprior = gamma 20  100
*tauprior = invgamma 3.0 0.4

thetaprior = gamma 8 2000 # gamma(a, b) for theta (estimate theta)

locusrate = 3 20 1000000

# finetune for GBtj, GBspr, theta, tau, mix, locusrate, seqerr
finetune =  1: 5 0.001 0.001  0.001 0.3 0.33 1.0  

# MCMC samples, locusrate, heredityscalars, Genetrees
print = 1 0 0 0   * 
burnin = 10000
*burnin = 8000
sampfreq = 4
nsample = 100000
*nsample = 1000
