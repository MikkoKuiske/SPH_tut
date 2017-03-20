# SPH_tut

- folder src contains .cpp and .hh files
- makefile is for compiling the program. Use command "make" to in master folder to create the executable
- folder data contains data created from running the executable
- folder build contains files from creating the program. Use "make clean" in master folder to clear these

-It is possible to visualize the data generated from executable by running program gnuplot and inside it running command "plot 'data/datafile.dat' i 0 using 1:2 with points pointtype 7 ps 2 lc rgb "green" title columnheader(1), 'data/datafile.dat' i 1 using 1:2 with points pointtype 7 ps 2 lc rgb "blue" title columnheader(1)"



- HOW TO USE GIT AND GET PROGRAM (Linux terminal):
You will be using commands

1) git clone https://github.com/MikkoPirttiniemi/SPH_tut.git
2) git fetch
3) git status
4) git pull
5) git add filename_1 filename_2 filename_3
6) git commit -m "write message here"
7) git push

Below are explanations to everything 

1) To clone everything via command line in :
git clone https://github.com/MikkoPirttiniemi/SPH_tut.git

2) To check if you are updated to latest:
git fetch
git status

OUTPUTS:
a) "On branch master
Your branch is up-to-date with 'origin/master'.
nothing to commit, working directory clean"
--> You are ok with everything.

b) "On branch master
Your branch is up-to-date with 'origin/master'.
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   filename_you_have_modified

no changes added to commit (use "git add" and/or "git commit -a")"
--> You are ahead of git database and probably have to upload your files to git database.

c) "On branch master
Your branch is behind 'origin/master' by 1 commit, and can be fast-forwarded.
  (use "git pull" to update your local branch)
nothing to commit, working directory clean"
--> You are behind git database and should use "git pull" to update your local data.



3) To update your database:
git pull

4) To add a new file to repository:
git add filename_1 filename_2 filename_3
git commit -m "write message here"
git push