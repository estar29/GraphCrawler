# Running on Terminal.
1. Navigate to the directory where the program is using cd commands.

2. When in the correct directory, type in the following to configure the Makefile.

      make graph-crawler

3. To run the program with your own commands, type in the following:

      ./graph-crawler <"root node"> <depth limit>

      Ensure that the root node is spelled correctly and is spaced, and the depth limit is an integer value.



# Running on Centaurus
1. Ensure that you are either on UNC-Charlotte's campus or connected to the VPN to access Centaurus.

2. Type the following to access the Centaurus partition:

     ssh unccEmail@hpc-student.uncc.edu

3. Type in your UNCC password and perform your 2FA.

4. git clone the repository to your Centaurus Partition via SSH.

     Note: Copying and pasting a generate SSH key/token may be required to authenticate yourself.

5. Type the following command to queue the Slurm job:

      sbatch gc-slurm.sh

6. Wait about 5-7 minutes for the output to be generated.

7. To view the output of the Slurm job, cat the generated output file.

      Note: the output for the job may be too large to fit on the Terminal Screen.
