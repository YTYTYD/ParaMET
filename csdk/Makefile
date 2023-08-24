CC = mpicc
FLAG = -lm
INCL = -I ./sdkinclude -I ./
SDKSRC = ./sdksrc/admissionsPRead.c ./sdksrc/consts.c ./sdksrc/drugExtract.c ./sdksrc/drugRead.c ./sdksrc/eventsRead.c ./sdksrc/featureExtract.c ./sdksrc/fextract.c ./sdksrc/globals.c ./sdksrc/icdCodes.c ./sdksrc/icustaysExtractPRead.c ./sdksrc/idExtract.c ./sdksrc/init.c ./sdksrc/noteExtract.c ./sdksrc/noteRead.c ./sdksrc/patientsPRead.c ./sdksrc/sdkinit.c ./sdksrc/sInfoExtract.c ./sdksrc/taskInit.c ./sdksrc/transfersPRead.c ./sdksrc/utils.c

all:extracter

extracter:main.c $(SDKSRC)
	$(CC) $? $(FLAG) $(INCL) -o $@

debug:main.c $(SDKSRC)
	$(CC) $? -g $(FLAG) $(INCL) -o dbgextracter

clean:
	rm extracter
	rm dbgextracter