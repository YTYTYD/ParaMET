# ParaMET

ParaMET: A Parallel Framework for Efficient Medical Data Extraction on Tianhe-NG Supercomputer



### Compile the extraction program：

1. Make sure that openmpi is installed on your device. We are using openmpi version 4.1.4.

2. Open `defines.h`, located in the `extracter` folder of the program source code. Depending on the version of the MIMIC dataset you need to extract, add macro definitions: `#define MIMICIV` or `#define MIMICIII`.

3. Open the file `/src/const.c` and modify the directory configurations as shown in the following example:

   ```c
   char *OUTPUT_DIR = "/test/log/";  //log output path
   #ifdef MIMICIII
   char *data_path = "/abc/mimiciii/";	//Directory containing the raw CSV files of the MIMIC-III dataset.
   #elif defined MIMICIV
   char *data_path = "/abc/mimiciv/";	//Directory containing the raw CSV files of the MIMIC-IV dataset.
   #endif
   ```

4. Compile by entering the `make` command, and you will obtain an executable file named `extracter`.



### python SDK：

1. Copy the `extsdk` folder to the directory where your Python code is located. The `extsdk` folder should be inside the `pythonsdk` directory.
2. Run the previously compiled `extracter` executable file.

3. Write code to perform the extraction process. Refer to the `demo.py` file for specific code examples.



### Embedded SDK：

1. Modify the `define.h` and `const.c` files in the same way as the extraction program.
2. Write code for data extraction and processing. For specific examples of extraction code, refer to the `/csdk/main.c` file.