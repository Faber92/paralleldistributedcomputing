# paralleldistributedcomputing
Some projects about Parallel and Distributed Computing.

Projects:

-- CUDA Matrix Multiplication (CUDA MM): a CUDA implementation of MM\* using NVIDIA GPUs.

-- Matrix Multiplication (MM): few different implementations of MM\*, varying indices order.

-- SUMMA: an implementation of the Scalable Universal Matrix Multiplication Algorithm.

-- Threaded Matrix Multiplication (TMM): an implementation of MM\*, using threads.

All the above projects are modules themselves. Any of them references each other in a cascade usage. Indeed, TMM uses the best implementation of MM, and SUMMA uses TMM.

MM\*: Matrix Multiplication
