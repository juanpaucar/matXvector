// Produceto matriz - vector: SECUENCIAL
// Procesar por FILAS

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

main (int argc, char *argv[]) {
  struct timeval ts, tf;
  int i, j, N, M;
  int **a, *b, *c;

  if (argc != 3) {printf ("USO: %s <dimX> <dimY>\n", argv[0]); exit (1);}
  N = atoi(argv[1]);
  M = atoi(argv[2]);

  a = malloc (N * sizeof (int *));
  b = malloc (M * sizeof (int));
  c = malloc (N * sizeof (int));
  if (a==NULL || b==NULL || c==NULL) { printf ("Memoria\n"); exit (1);}

  for (i=0; i<N; i++) {
    a[i] = malloc (M * sizeof (int));
    if (a[i] == NULL) { printf ("Memoria\n"); exit (1);}
  }

  srandom (177845);
  for (i=0; i<N; i++)
    for (j=0; j<M; j++)
      a[i][j] = random() % 10;
 
  for (j=0; j<M; j++)
    b[j] = random() % 10;
 
  gettimeofday (&ts, NULL);
  for (i=0; i<N; i++) {
    c[i] = 0;
    for (j=0; j<M; j++)
      c[i] = c[i] + a[i][j] * b[j];
  }

  gettimeofday (&tf, NULL);
  printf ("Time: %f secs\n", ((tf.tv_sec - ts.tv_sec)*1000000u + 
           tf.tv_usec - ts.tv_usec)/ 1.e6);
 for (i=0; i<5; i++) {
    for (j=0; j<5; j++)
      printf ("%d ", a[i][j]);
    printf (" . . . ");
    for (j=M-5; j<M; j++)
      printf ("%d ", a[i][j]);
    printf ("\n");
  }
  printf ("           . . . \n");
  printf ("           . . . \n");
  for (i=N-5; i<N; i++) {
    for (j=0; j<5; j++)
      printf ("%d ", a[i][j]);
    printf (" . . . ");
    for (j=M-5; j<M; j++)
      printf ("%d ", a[i][j]);
    printf ("\n");
  }
  exit (0);
}
