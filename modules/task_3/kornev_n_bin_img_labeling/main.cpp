// Copyright 2019 Kornev Nikita

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <time.h>
#include <iostream>
#include "./bin_img_labeling.h"

TEST(broadcast, test1) {
  int rank, size, count = 7;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  image img(8, 8);

  if (rank == 0) {
    img.data[0][0] = 1;
    img.data[0][1] = 1;
    img.data[0][2] = 1;
    img.data[0][3] = 1;
    img.data[0][5] = 1;
    img.data[0][6] = 1;
    img.data[1][5] = 1;
    img.data[1][6] = 1;
    img.data[2][5] = 1;
    img.data[2][6] = 1;
    img.data[2][1] = 1;
    img.data[2][2] = 1;
    img.data[4][2] = 1;
    img.data[4][4] = 1;
    img.data[5][2] = 1;
    img.data[5][4] = 1;
    img.data[5][6] = 1;
    img.data[5][1] = 1;
    img.data[6][1] = 1;
    img.data[6][2] = 1;
    img.data[6][6] = 1;
    img.data[7][4] = 1;
    img.data[7][6] = 1;
    img.data[7][7] = 1;
  }

  labeling(img);

  if (rank == 0) {
    ASSERT_EQ(count, img.count);
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
