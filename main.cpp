#include <iostream>
#include <boost/mpi.hpp>
#include <random>
#include <memory>

namespace mpi = boost::mpi;

const int MAX_NUMBERS = 100;

void rootProcess(int &number_amount, mpi::environment &environment,
                 mpi::communicator &communicator) {
    int arr[MAX_NUMBERS];
    for (int i = 0; i < MAX_NUMBERS; ++i) {
        arr[i] = i*i;
    }

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, MAX_NUMBERS);

    number_amount = static_cast<int>(dist(rng));
    communicator.send(1, 0, arr, number_amount);
    std::cout << "0 sent " << number_amount << " numbers to 1" << std::endl;
}

void childProcess(int &number_amount, mpi::environment &environment,
                 mpi::communicator &communicator) {
    mpi::status status = communicator.probe(0, 0);
    if (!status.count<int>().has_value()) {
        return;
    }

    number_amount = status.count<int>().get();
    auto arr = std::make_unique<int[]>(number_amount);
    communicator.recv(0,0, arr.get(), number_amount);

    std::cout << "1 got integer array with size =" << number_amount << std::endl;
    for (int i = 0; i < number_amount; ++i) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    int number_amount;
    mpi::environment environment(argc, argv);
    mpi::communicator world;
    std::cout << "Process with rank " << world.rank() << " was initialized." << std::endl;
    if (world.rank() == 0) {
        rootProcess(number_amount, environment, world);
    } else if (world.rank() == 1) {
        childProcess( number_amount, environment, world);
    }
    return 0;
}
