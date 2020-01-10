// Copyright 2019 Kornev Nikita

#include "../../../modules/task_2/kornev_n_broadcast/broadcast.h"
#include <iostream>

void broadcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank != root) {
    MPI_Status status;
    MPI_Recv(buffer, count, datatype, MPI_ANY_SOURCE, 0, comm, &status);
  }

  if (rank * 2 + 1 <= size - 1) {
    int left = rank * 2 + 1, right = left;
    if (left + 1 <= size - 1) {
      right = left + 1;
    }

    for (int i = left; i <= right; i++) {
      MPI_Send(buffer, count, datatype, i, 0, comm);
    }
  }
}




