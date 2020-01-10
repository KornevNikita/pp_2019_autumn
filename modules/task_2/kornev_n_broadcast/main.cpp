// Copyright 2019 Kornev Nikita

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <time.h>
#include <iostream>
#include "./broadcast.h"

TEST(broadcast, int) {
  int rank, size, root = 0, n;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  n = size;
  int* a = new int[n];
  int sum = 0;

  if (rank == root) {
    for (int i = 0; i < n; i++) {
      a[i] = 0;
    }
  }

  // double start1, end1, start2, end2;
  // start1 = MPI_Wtime();
  broadcast(&a[0], n, MPI_INT, root, MPI_COMM_WORLD);
  // end1 = MPI_Wtime();

  // start2 = MPI_Wtime();
  // MPI_Bcast(&a, n, MPI_INT, root, MPI_COMM_WORLD);
  // end2 = MPI_Wtime();

  a[rank] = 1;
  MPI_Reduce(&a[rank], &sum, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
  if (rank == root) {
    // std::cout << "my_bcast " << end1 - start1 << std::endl;
    // std::cout << "mpi_bcast " << end2 - start2 << std::endl;
    // std::cout << (end2 - start2) / (end1 - start1) << std::endl;
    ASSERT_EQ(size, sum);
  }
}

TEST(broadcast, float) {
  int rank, size, root = 0, n;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  n = size;
  float* a = new float[n];
  float sum = .0;

  if (rank == root) {
    for (int i = 0; i < n; i++) {
      a[i] = .0;
    }
  }

  broadcast(&a[0], n, MPI_FLOAT, root, MPI_COMM_WORLD);
  a[rank] = 1.;
  MPI_Reduce(&a[rank], &sum, 1, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);

  if (rank == root) {
    ASSERT_EQ(sum, size);
  }
}

TEST(broadcast, double) {
  int rank, size, root = 0, n;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  n = size;
  double* a = new double[n];
  double sum = .0;

  if (rank == root) {
    for (int i = 0; i < n; i++) {
      a[i] = .0;
    }
  }

  broadcast(&a[0], n, MPI_DOUBLE, root, MPI_COMM_WORLD);
  a[rank] = 1.;
  MPI_Reduce(&a[rank], &sum, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);

  if (rank == root) {
    ASSERT_EQ(sum, size);
  }
}

TEST(broadcast, test4) {
  int rank, size, root = 0, n;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  n = size;
  int* a = new int[n];
  int sum = 0, res = 0;

  if (rank == root) {
    for (int i = 0; i < n; i++)
      a[i] = 1;
  }

  broadcast(&a[0], n, MPI_INT, root, MPI_COMM_WORLD);

  if (rank != 0) {
    for (int i = 0; i < n; i++) {
      sum += a[i];
    }
  }

  MPI_Reduce(&sum, &res, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
  if (rank == 0) {
    ASSERT_EQ(res, (size - 1) * n);
  }
}

TEST(broadcast, test5) {
  int rank, size, root = 0, n = 100;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int* a = new int[n];
  int sum = 0;

  if (rank == root) {
    for (int i = 0; i < n; i++) {
      a[i] = 0;
    }
  }

  broadcast(&a[0], n, MPI_INT, root, MPI_COMM_WORLD);
  a[rank] = 1;
  MPI_Reduce(&a[rank], &sum, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

  if (rank == root) {
    ASSERT_EQ(sum, size);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
