//
// Created by Дмитрий Бутилов on 06.05.18.
//
#include <mpi.h>
#include <iostream>

int rank, size;

void calculateCapacity(int repeats, int lenght) {
    double time = 0;
    auto *message = (unsigned char *) malloc(lenght * sizeof(unsigned char));
    MPI_Status state;
    for (int j = 0; j < repeats; j++) {
        if (rank == 0) {
            for (int i = 1; i < size; i++) {
                double startTime = MPI_Wtime();
                MPI_Send(message, lenght, MPI_BYTE, i, 3, MPI_COMM_WORLD);
                MPI_Recv(message, lenght, MPI_BYTE, i, 4, MPI_COMM_WORLD, &state);
                time += MPI_Wtime() - startTime;
            }
        } else {
            MPI_Recv(message, lenght, MPI_BYTE, 0, 3, MPI_COMM_WORLD, &state);
            MPI_Send(message, lenght, MPI_BYTE, 0, 4, MPI_COMM_WORLD);
        }
    }
    free(message);
    if (rank == 0) {
        auto resultCapacity = 2.0 * repeats * lenght / time; // C = 2N*L/T
        std::cout << "Capacity: " << resultCapacity << "\nRepeats: " << repeats << "\nLenght: = " << lenght << "\n";
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int repeats = 50;
    int lenght = 8 * 1024 * 1024;
    if (rank == 0) {
        std::cout << "Start\n";
    }

    double time = MPI_Wtime();
    calculateCapacity(repeats, lenght);
    time = MPI_Wtime() - time;

    if (rank == 0) {
        std::cout << "End\nTime: " << time << " seconds\n";
    }

    MPI_Finalize();
    return 0;
}