#define _BSD_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define debug(format, ...) \
    fprintf(stderr, format " %s %d", ##__VA_ARGS__, __FILE__, __LINE__)

int main() {

    size_t global;
    size_t local;

    int err;
 
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel1, kernel2;

    err = clGetPlatformIDs(1, &platform_id, NULL);
    if (err != CL_SUCCESS)
    {   
        debug("Error: Failed to get a platform!\n");
        return EXIT_FAILURE;
    }


    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS)
    {   
        debug("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }

    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context)
    {
        debug("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }

    queue = clCreateCommandQueue(context, device_id, 0, &err);
    if (!queue)
    {
        debug("Error: Failed to create a command queue!\n");
        return EXIT_FAILURE;
    }

    FILE *handle = fopen("kernel.cl", "r");
    fseek(handle, 0, SEEK_END);
    size_t program_size = ftell(handle);
    rewind(handle);

    char* program_buffer = (char *) malloc(program_size + 1);
    program_buffer[program_size] = '\0';
    fread(program_buffer, sizeof(char), program_size, handle);
    fclose(handle);
    program = clCreateProgramWithSource(context, 1,
            (const char**) &program_buffer, &program_size, NULL);
    if (!program)
    {
        debug("Error: Failed to create compute program!\n");
        return EXIT_FAILURE;
    }

    free(program_buffer);

    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];
 
        debug("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        debug("%s\n", buffer);
        exit(1);
    }
    kernel1 = clCreateKernel(program, "fn1", &err);
    if (!kernel1 || err != CL_SUCCESS)
    {
        debug("Error: Failed to create compute kernel!\n");
        exit(1);
    }

    kernel2 = clCreateKernel(program, "fn2", &err);
    if (!kernel2 || err != CL_SUCCESS)
    {
        debug("Error: Failed to create compute kernel!\n");
        exit(1);
    }


 

    std::string line;
    // std::getline(std::cin, line);
    // int height = std::stoi(line, nullptr, 0);
    // std::getline(std::cin, line);
    // int width  = std::stoi(line, nullptr, 0);
    int height, width;
    scanf("%d\n", &height);
    scanf("%d\n", &width);
    std::cerr << "(Height, Width): " << height << ", " << width << std::endl;

    cl_mem im0_buf = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(float) * height * width, NULL, NULL);
    cl_mem im1_buf = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(float) * height * width, NULL, NULL);
    cl_mem Ix = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(float) * height * width, NULL, NULL);
    cl_mem Iy = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(float) * height * width, NULL, NULL);
    cl_mem It = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(float) * height * width, NULL, NULL);
    cl_mem denom = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(float) * height * width, NULL, NULL);
    cl_mem u = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(float) * height * width, NULL, NULL);
    cl_mem v = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(float) * height * width, NULL, NULL);
    cl_mem u_new = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(float) * height * width, NULL, NULL);
    cl_mem v_new = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(float) * height * width, NULL, NULL);

    float im[width * height];
    char _format[width*height+5];
    // read first image
    char img_str[width*height*100];
    char *token;
    char *buffer;
    if(!gets(img_str)) {
      exit(1);
    }
    token = strtok_r(img_str, " ",&buffer);
    for (int i = 0; i < width*height; i++) {
        int ret = sscanf(token, "%f", im + i);
        token = strtok_r(NULL, " ",&buffer);
        if(ret != 1) {
          fprintf(stderr, "out of input:%s\n",token);
          exit(2);
        }
    }
    fprintf(stderr, "OpenCL Read file\n");

    // std::getline(std::cin, line);
    // bool result = im0->ParseFromString(line);
    err = clEnqueueWriteBuffer(queue, im0_buf, CL_TRUE, 0, sizeof(float) * height * width, im, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        debug("Error: Failed to set write buffer! %d\n", err);
        exit(1);
    }

    err = 0;
    err |= clSetKernelArg(kernel1, 0, sizeof(cl_mem), &It);
    err |= clSetKernelArg(kernel1, 1, sizeof(cl_mem), &Ix);
    err |= clSetKernelArg(kernel1, 2, sizeof(cl_mem), &Iy);
    err |= clSetKernelArg(kernel1, 3, sizeof(cl_mem), &denom);
    if (err != CL_SUCCESS)
    {
        debug("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }
    err = clGetKernelWorkGroupInfo(kernel1, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    if (err != CL_SUCCESS)
    {
        debug("Error: Failed to retrieve kernel work group info! %d\n", err);
        exit(1);
    }

    err = 0;
    err |= clSetKernelArg(kernel2, 0, sizeof(cl_mem), &It);
    err |= clSetKernelArg(kernel2, 1, sizeof(cl_mem), &Ix);
    err |= clSetKernelArg(kernel2, 2, sizeof(cl_mem), &Iy);
    err |= clSetKernelArg(kernel2, 3, sizeof(cl_mem), &denom);
    err |= clSetKernelArg(kernel2, 4, sizeof(cl_mem), &u);
    err |= clSetKernelArg(kernel2, 5, sizeof(cl_mem), &u_new);
    err |= clSetKernelArg(kernel2, 6, sizeof(cl_mem), &v);
    err |= clSetKernelArg(kernel2, 7, sizeof(cl_mem), &v_new);

    if (err != CL_SUCCESS)
    {
        debug("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }

    cl_mem prev = im0_buf;
    cl_mem curr = im1_buf;

    while (true) {
        clSetKernelArg(kernel1, 4, sizeof(cl_mem), &prev);
        clSetKernelArg(kernel1, 5, sizeof(cl_mem), &curr);
        char img_str[width*height*100];
        char *token;
        char *buffer;
        if(!gets(img_str)) {
          exit(1);
        }
        token = strtok_r(img_str, " ",&buffer);
        for (int i = 0; i < width*height; i++) {
            if(token == NULL) {
              fprintf(stderr, "token is null\n");
              exit(3);
            }
            int ret = sscanf(token, "%f", im + i);
            token = strtok_r(NULL, " ",&buffer);
            if(ret != 1) {
              fprintf(stderr, "out of input:%s\n",token);
              exit(2);
            }
        }
        sleep(1);
        fprintf(stderr, "OpenCL Read file\n");
        err = clEnqueueWriteBuffer(queue, curr, CL_TRUE, 0, sizeof(float) * height * width, im, 0, NULL, NULL);
        global = height * width;
        err = clEnqueueNDRangeKernel(queue, kernel1, 1, NULL, &global, &local, 0, NULL, NULL);
        if (err)
        {
            debug("Error: Failed to execute kernel!\n");
            return EXIT_FAILURE;
        }
        clFinish(queue);
        for (int i = 0; i < 1; i++) {
            err = clEnqueueNDRangeKernel(queue, kernel2, 1, NULL, &global, &local, 0, NULL, NULL);
            if (err)
            {
                debug("Error: Failed to execute kernel!\n");
                return EXIT_FAILURE;
            }
            clFinish(queue);
            cl_mem v_tmp = v;
            v = v_new;
            v_new = v_tmp;
            cl_mem u_tmp = u;
            u = u_new;
            u_new = u_tmp;
            clSetKernelArg(kernel2, 4, sizeof(cl_mem), &u);
            clSetKernelArg(kernel2, 5, sizeof(cl_mem), &u_new);
            clSetKernelArg(kernel2, 6, sizeof(cl_mem), &v);
            clSetKernelArg(kernel2, 7, sizeof(cl_mem), &v_new);
        }

        err = clEnqueueReadBuffer(queue, u, CL_TRUE, 0, sizeof(float) * height * width, im, 0, NULL, NULL);
        for (int i = 0; i < height * width; i++) {
            std::cout << im[i] << " ";
        }
        std::cout << std::endl;

        err = clEnqueueReadBuffer(queue, v, CL_TRUE, 0, sizeof(float) * height * width, im, 0, NULL, NULL);
        for (int i = 0; i < height * width; i++) {
            std::cout << im[i] << " ";
        }
        std::cout << std::endl;

        cl_mem tmp = curr;
        curr = prev;
        prev = tmp;
    }
}
