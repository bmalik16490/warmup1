+-------+

| BUILD |

+-------+



Comments: to build the files type: 

		

		make

	  

	  to clean type:

		

		make clean

	

	  to run the executable type:



		./warmup1 sort filename

	  or

		cat filename | ./warmup1 sort



	where filename is the name of the ASCII text file from which data is read. Each valid line contains four fields with a tab separating each field from the other

	eg:

		sign<tab>timestamp<tab>amount<tab>description	


+----------+


| GRADING  | 


+----------+


a)doubly linked circular list: 40 points

(b.1) Sort (file): 30 points
(b.2) Sort (stdin):30 points
missing required sections in readme file: none
cannot compile:no comments
make clean: cleans all .o files and executable
segmentation faults : none so far

malformed input: each valid input line contains exactly four fields as described above. For each malformed input an error message is displayed 
too slow: no comments
bad commandline: usage information is diplayed on entering a wrong command

Bad behaviour for random input: handled

| BUGS |

+------+



Comments: No bugs



+-------+

| OTHER |

+-------+



Comments on design decisions: Used bubble sort for sorting the link list

Comments on deviation from spec: None



