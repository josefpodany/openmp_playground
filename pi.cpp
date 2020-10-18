#include <string>
#include <iostream>

#include <omp.h>

static long num_steps = 1000000000;

void sequential() {
    int i;
    double x, pi, sum = 0.0;

    double step1 = 1.0/(double)num_steps;

    for( i = 0; i < num_steps; i++ ) {
        x = ( i + 0.5 ) * step1;
        sum = sum + 4.0/( 1.0+x*x );
    }
    pi = step1 * sum;
    std::cout << "[sequential] pi: " << std::to_string(pi) << std::endl;
}

#define NUM_THREADS 64
#define PAD         8

void parallel_for() {
    int i;
    double pi, sum = 0.0;

    double step1 = 1.0/(double)num_steps;
#pragma omp parallel
    {
        double x;
#pragma omp for reduction(+:sum)
        for( i = 0; i < num_steps; i++ ) {
            x = ( i + 0.5 ) * step1;
            sum = sum + 4.0/( 1.0+x*x );
        }
    }
    pi = step1 * sum;
    std::cout << "[parallel] pi: " << std::to_string(pi) << " (parallel for reduction)" << std::endl;
}

void parallel() {
    double step = 1.0/(double)num_steps;
    double sum[NUM_THREADS][PAD];
    
    omp_set_num_threads(NUM_THREADS);

    int threads = omp_get_num_threads();
#pragma omp parallel
    {
        double x;
        int thread_id = omp_get_thread_num();
        int n_threads = omp_get_num_threads();
        sum[thread_id][0] = 0.0;
    
        if ( thread_id == 0 ) threads = n_threads;

        for ( int i = thread_id; i < num_steps; i += threads ) {
            x = ( i + 0.5 ) * step;
            sum[thread_id][0] += + 4.0/( 1.0+x*x );
        }
    }

    double pi = 0.0;
    for ( int i = 0; i < NUM_THREADS; i++ ) pi += sum[i][0] * step;
    std::cout << "[parallel] pi: " << std::to_string(pi) << " (adjusted for cache lines)" << std::endl;
}

void parallel_atomic() {
    double step = 1.0/(double)num_steps;
    double sum = 0.0;
    
    omp_set_num_threads(NUM_THREADS);
    
    int threads = omp_get_num_threads();
#pragma omp parallel
    {
        double x;
        double tmp = 0.0;
        int thread_id = omp_get_thread_num();
        int n_threads = omp_get_num_threads();
    
        if ( thread_id == 0 ) threads = n_threads;

        for ( int i = thread_id; i < num_steps; i += threads ) {
            x = ( i + 0.5 ) * step;
            tmp += + 4.0/( 1.0+x*x );
        }

#pragma omp atomic
        sum += tmp;

    }

    double pi = sum * step;
    std::cout << "[parallel] pi: " << std::to_string(pi) << " (atomic)" << std::endl;
}



template <typename F>
void time_it(F func()) {
    double start = omp_get_wtime();
    func();
    double end = omp_get_wtime();
    std::cout << "took " << end - start << std::endl;
}

int main() {
    time_it(sequential);
    time_it(parallel);
    time_it(parallel_atomic);
    time_it(parallel_for);
}
