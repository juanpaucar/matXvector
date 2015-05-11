// Producto matriz - vector: COMBINACION LINEAL
// Bloques de filas. Usa threads;  necesita control de concurrencia

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>

typedef struct args {
  int thID;
  int min;
  int max;
} arg;

int **a, *b, *c;
int N, M, T;
sem_t mx;

void tres (arg *args) {
  int i, j;
  int *cl;

  cl = malloc (N * sizeof (int));
  for (i=0; i<N; i++) cl[i] = 0;
  for (j=args->min; j<args->max; j++) 
    for (i=0; i<N; i++)
      cl[i] = cl[i] + a[i][j] * b[j];

  for (i=0; i<N; i++) {
    sem_wait(&mx);
    c[i]+= cl[i];
    sem_post(&mx);
  }
}

main (int argc, char *argv[]) {
  struct timeval ts, tf;
  int i, j;
  int cl, fr;
  pthread_t *th;
  arg *args;

  if (argc != 4){printf("USO: %s <dimX> <dimY> <#hilos>\n", argv[0]); exit (1);}

  N = atoi(argv[1]);
  M = atoi(argv[2]);
  T = atoi(argv[3]);

  th = malloc (T * sizeof (pthread_t)); 
  a = malloc (N * sizeof (int *));
  b = malloc (M * sizeof (int));
  c = malloc (N * sizeof (int));
  args = malloc (T * sizeof (arg));
  if (a==NULL || th==NULL || args==NULL || b==NULL || c==NULL) { 
    printf ("Memoria\n"); 
    exit (1);
  }

  for (i=0; i<N; i++) {
    a[i] = malloc (M * sizeof (int));
    if (a[i] == NULL) { printf ("Memoria\n"); exit (1);}
  }

  srandom (177845);
  for (i=0; i<N; i++) {
    c[i] = 0;
    for (j=0; j<M; j++)
      a[i][j] = random() % 10;
  }

  for (j=0; j<M; j++)
    b[j] = random() % 10;

  sem_init(&mx, 0, 1);
  cl = (int)ceil((float)M/(float)T);
  fr = (int)floor((float)M/(float)T);

  for (j=0; j<M%T; j++) {
    args[j].thID = j;
    args[j].min = j*cl;
    args[j].max = (j+1)*cl;
  }

  for (j=M%T; j<T; j++) {
    args[j].thID = j;
    args[j].min = (M%T)*cl + (j-(M%T))*fr;
    args[j].max = (M%T)*cl + (j-(M%T)+1)*fr;
  }

  gettimeofday (&ts, NULL);
  for (i=0; i<T; i++)
    pthread_create (&th[i], NULL, (void *) tres, (void *)&args[i]);
  
  for (i=0; i<T; i++)
    if (pthread_join (th[i], NULL)) {printf ("PJOIN (%d)\n", i); exit (1);};
  gettimeofday (&tf, NULL);
  sem_destroy(&mx);
  printf ("Time (col): %f secs\n", ((tf.tv_sec - ts.tv_sec)*1000000u +
           tf.tv_usec - ts.tv_usec)/ 1.e6);
  /*for (i=0; i<N; i++) {*/
    /*printf("%d", c[i]);*/
  /*}*/
  exit (0);
}

