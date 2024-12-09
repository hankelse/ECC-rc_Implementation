
import sys


#Get run name
jobname = input("Enter Job Name: ")

#Get memory
memory = input("Enter Memory Needed in GB (default 50): ")
if memory == "":
    memory = "50"


#Get time
time = input("Enter your runtime in hours (default 1): ")
if time == "":
    time = "01"
if len(time) == 1:
    time = "0" + time


#Get command to exec
command = input("What command do you want to run? ")

#get file to write to
# outpath = input("jobfile name? ")
outpath = sys.argv[1]
print("created jobfile: " + outpath)


jobfilestring = f'''
#PBS -N {jobname}
#PBS -l nodes=1:ppn=1
#PBS -l mem={memory}GB
#PBS -q una-new
#PBS -M helsesse@hamilton.edu
#PBS -m bea
#PBS -j oe
#PBS -l walltime={time}:00:00
#PBS -r n


echo ""
echo "Running data on all (including big)"
echo ""

module load gcc/14.2.0

cd $PBS_O_WORKDIR

{command} >> log.txt

echo -n "Finished at: "
date

'''


file = open("jobfiles/"+outpath+".run", "w")
file.write(jobfilestring)
file.close()