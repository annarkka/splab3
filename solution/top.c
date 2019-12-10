#include <stdio.h>

int main{
  int A[2]; // pipe A
	pipe(A);
	pid_t pid1 = fork(); // cat
	if (!pid1) {
		dup2(A[1], 1);
		close(A[0]);
		close(A[1]);
		execlp("cut","cut", "-d " , "-f12",  "log.txt", NULL); //cut -d' ' -f10,12 log.txt
		exit(EXIT_FAILURE);
	}
	
	int B[2]; // pipe B
	pipe(B);
	pid_t pid2 = fork();                                       
	if (!pid2) { // cut
		dup2(A[0], 0);
		close(A[0]);
		close(A[1]); 
		dup2(B[1], 1);
		close(B[0]);
		close(B[1]);      

		execlp("sed","sed","s/\"//",  NULL);//cut -d'/' -f1
		exit(EXIT_FAILURE);                            
	}

	close(A[0]);
	close(A[1]);
	int C[2]; // pipe C
	pipe(C);
	pid_t pid3 = fork();                                       
	if (!pid3) { // sed
		dup2(B[0], 0);
		close(B[0]);
		close(B[1]);

		dup2(C[1], 1);
		close(C[0]);
		close(C[1]);

		execlp("sort","sort", "-s", NULL); //sort -s 
		exit(EXIT_FAILURE);                            
	}
	close(B[0]);
	close(B[1]);
	int D[2]; // pipe C
	pipe(D);
	pid_t pid4 = fork();   
	if (!pid4) { // awk
		dup2(C[0], 0);
		close(C[0]);
		close(C[1]);

		dup2(D[1], 1);
		close(D[0]);
		close(D[1]);

		execlp("uniq","uniq", "-c",NULL); //  uniq -c
		exit(EXIT_FAILURE);                            
	}
	close(C[0]);
	close(C[1]);
	int E[2]; // pipe C
	pipe(E);
	pid_t pid5 = fork();   
	if (!pid5) { // awk
		dup2(D[0], 0);
		close(D[0]);
		close(D[1]);
		
		dup2(E[1], 1);
		close(E[0]);
		close(E[1]);

		execlp("sort","sort", "-nr" , NULL); //   sort -nr
		exit(EXIT_FAILURE);                            
	}

	close(D[0]);
	close(D[1]);
	int F[2]; // pipe C
	pipe(F);
	pid_t pid6 = fork();       //           
	if (!pid6) { // sed
		dup2(E[0], 0);
		close(E[0]);
		close(E[1]);

		dup2(F[1], 1);
		close(F[0]);
		close(F[1]);
		execlp("head","head","-n", "5", NULL); // head -n 4
		exit(EXIT_FAILURE);                            
	}
	close(E[0]);
	close(E[1]);


	int pid7 = fork();
	if (!pid7) { // sed
		dup2(F[0], 0);
		close(F[0]);
		close(F[1]);//
		execlp("awk","awk", "{i+=1;printf i \".\" ;print( $2,\" \"  $1, \" - \" (($1/5880)*100) \" %\" )  }", NULL); // 
		exit(EXIT_FAILURE);                            
	}
	close(F[0]);
	close(F[1]);
	waitpid(pid7, 0, 0);
}
