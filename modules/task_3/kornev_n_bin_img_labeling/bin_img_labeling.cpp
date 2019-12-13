// Copyright 2019 Kornev Nikita

#include "../../../modules/task_3/kornev_n_bin_img_labeling/bin_img_labeling.h"
void labeling(image& img)
{
  int rank, size, cur_num, string_count, new_string_count, rest, flag = 0;
  int* global_img_arr, *local_img_arr;
  image* local_img;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status status;
  string_count = img.m / size;
  rest = img.m % size;
  std::cout << "hello im proc" << rank << std::endl;

  if (rank == 0) {
    new_string_count = string_count + rest;
  } else {
    new_string_count = string_count;
  }

  // alloc mem for recv buf & local_img & res_buf 
  global_img_arr = new int[img.m * img.n];
  local_img_arr = new int[new_string_count * img.n];
  local_img = new image(new_string_count, img.n);

  // clone img matrix values into arr & send parts to other ranks
  if (rank == 0) {
    for (int i = 0; i < img.m; i++) {
      for (int j = 0; j < img.n; j++) {
        global_img_arr[i * img.n + j] = img.data[i][j];
      }
    }

    // put first part of img arr in 0-rank's local img 
    for (int i = 0; i < (new_string_count) * img.n; i++) {
      local_img_arr[i] = global_img_arr[i];
    }

    // send other parts of img arr to other ranks
    for (int i = 1; i < size; i++) {
      MPI_Send(&global_img_arr[i * img.n * string_count + rest * img.n], string_count * img.n,
        MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  } else {
    MPI_Recv(&local_img_arr, string_count * img.n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  // clone part of img arr into cur rank's img
  for (int i = 0; i < new_string_count; i++) {
    for (int j = 0; j < img.n; j++) {
      local_img->data[i][j] = local_img_arr[i * img.n + j];
    }
  }

  // img labeling
  for (int i = 0; i < local_img->m; i++)
  {
    std::cout << "proc" << rank << " " << "i = " << i << std::endl;
    for (int j = 0; j < img.n; j++)
    {
      std::cout << "proc" << rank << " " << "j = " << j << std::endl;
      cur_num = img.count;
      if (local_img->data[i][j] == 1)
      {
        flag = 1;
        local_img->data[i][j] = cur_num;
        std::cout << "proc" << rank << " " << i << " " << j << std::endl;
        if (i + 1 < new_string_count && local_img->data[i + 1][j] == 1) {
          int k = i + 1;
          while (k < new_string_count && img.data[k][j]) {
            local_img->data[k][j] = cur_num;
            if (j - 1 >= 0 && local_img->data[k][j - 1]) {
              int l = j - 1;
              while (l >= 0 && local_img->data[k][l]) {
                local_img->data[k][l] = cur_num;
                std::cout << "proc" << rank << " " << k << " " << l << std::endl;
                l--;
              }
            }
            if (j + 1 < img.n && local_img->data[k][j + 1]) {
              int l = j + 1;
              while (l < img.n && local_img->data[k][l]) {
                local_img->data[k][l] = cur_num;
                std::cout << "proc" << rank << " " << k << " " << l << std::endl;
                l++;
              }
            }
            std::cout << "proc " << rank << " " << k << " " << j << std::endl;
            k++;
          }
        }
      }
      else {
        if (flag) {
          flag = 0;
          img.count++;
          std::cout << "proc" << rank << " " << "count+" << std::endl;
        }
      }
    }
    std::cout << "proc" << rank << " " << "end " << i << std::endl;
  }

  // now clone cur rank's img back to img arr
  std::cout << "loc img -> loc img arr" << std::endl;
  for (int i = 0; i < new_string_count; i++) {
    for (int j = 0; j < img.n; j++) {
      local_img_arr[i * img.n + j] = local_img->data[i][j];
    }
  }

  // put 0-ranks's local arr into global img arr
  if (rank == 0) {
    for (int i = 0; i < new_string_count * img.n; i++) {
      global_img_arr[i] = local_img_arr[i];
    }
  }

  // then send local img arrs to 0-rank
  std::cout << "loc -> 0-rank" << std::endl;
  if (rank != 0) {
      MPI_Send(&local_img_arr[0], new_string_count * img.n,
        MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  else {
    for (int i = 1; i < size; i++) {
      MPI_Recv(&global_img_arr[i * img.n * string_count + rest * img.n], new_string_count * img.n, 
        MPI_INT, i, 0, MPI_COMM_WORLD, &status);
    }
  }

  // now build new img
  std::cout << "building new img" << std::endl;
  if (rank == 0) {
    for (int i = 0; i < img.m; i++) {
      for (int j = 0; j < img.n; j++) {
        img.data[i][j] = global_img_arr[i * img.n + j];
      }
    }
    std::cout << img;
  }
  img.count--;
}
  



