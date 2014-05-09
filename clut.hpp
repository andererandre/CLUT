/*******************************************************************************
 * Copyright (c) 2012-2013 André Straubmeier
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 ******************************************************************************/

/*! \file
 *
 *   \brief The OpenCL Utility Toolkit (clut) is a collection of functions
 *          for a more comfortable handling of the OpenCL C++ bindings.
 *   \author André Straubmeier
 *
 *   \version 1.1.1
 *   \date Dec 2013
 */

/*! \mainpage
 * \section introduction Introduction
 *
 * This code is NOT meant to simplify your entrance into the world of OpenCL.
 * Getting started with the OpenCL C++ bindings is actually quite easy as-is
 * and I highly recommend having a look at the official introduction before
 * jumping head first into using the functions from the \em clut namespace. You
 * should have understood the meaning of things like contexts, platforms and
 * devices and at least the most basic OpenCL operations before simplifying the
 * everyday usage of these things by replacing parts of your code with the
 * simplified \em clut functions.
 *
 * You can think of \em clut being for OpenCL what glut is for OpenGL. It
 * shouldn't keep you from understanding the low-level basics of the runtime,
 * but it should keep you from writing the exact same code over and over again.
 *
 */

#ifndef CLUT_HPP_
#define CLUT_HPP_

#include "cl.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>

/*! \namespace clut
 *
 * \brief The utility toolkit for the OpenCL C++ bindings is defined within this namespace.
 *
 */
namespace clut
{
    // These defines are required because cl.hpp uses some weird
    // mix between typedef (string) and define (vector) for this,
    // leading to inconsistent behaviour concerning the namespace:
    // * cl::STRING_CLASS requires the "cl::" because it's a typedef
    // * cl::VECTOR_CLASS does not work with the "cl::" because it's a define
    // By defining these two keywords on our own we can fix this inconsistency.

    #if !defined(__USE_DEV_VECTOR) && !defined(__NO_STD_VECTOR)
    #define VECTOR_CLASS std::vector
    #elif !defined(__USE_DEV_VECTOR)
    #define VECTOR_CLASS cl::vector
    #endif

    #if !defined(__USE_DEV_STRING) && !defined(__NO_STD_STRING)
    #define STRING_CLASS std::string
    #elif !defined(__USE_DEV_STRING)
    #define STRING_CLASS cl::string
    #endif

    /*! \brief Converts an OpenCL error code to a descriptive string.
     *
     *  \param err The OpenCL error code.
     */
    STRING_CLASS errString(cl_int err)
    {
        switch (err)
        {
            case CL_SUCCESS:                         return "CL_SUCCESS";
            case CL_DEVICE_NOT_FOUND:                return "CL_DEVICE_NOT_FOUND";
            case CL_DEVICE_NOT_AVAILABLE:            return "CL_DEVICE_NOT_AVAILABLE";
            case CL_COMPILER_NOT_AVAILABLE:          return "CL_COMPILER_NOT_AVAILABLE";
            case CL_MEM_OBJECT_ALLOCATION_FAILURE:   return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
            case CL_OUT_OF_RESOURCES:                return "CL_OUT_OF_RESOURCES";
            case CL_OUT_OF_HOST_MEMORY:              return "CL_OUT_OF_HOST_MEMORY";
            case CL_PROFILING_INFO_NOT_AVAILABLE:    return "CL_PROFILING_INFO_NOT_AVAILABLE";
            case CL_MEM_COPY_OVERLAP:                return "CL_MEM_COPY_OVERLAP";
            case CL_IMAGE_FORMAT_MISMATCH:           return "CL_IMAGE_FORMAT_MISMATCH";
            case CL_IMAGE_FORMAT_NOT_SUPPORTED:      return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
            case CL_BUILD_PROGRAM_FAILURE:           return "CL_BUILD_PROGRAM_FAILURE";
            case CL_MAP_FAILURE:                     return "CL_MAP_FAILURE";
            case CL_INVALID_VALUE:                   return "CL_INVALID_VALUE";
            case CL_INVALID_DEVICE_TYPE:             return "CL_INVALID_DEVICE_TYPE";
            case CL_INVALID_PLATFORM:                return "CL_INVALID_PLATFORM";
            case CL_INVALID_DEVICE:                  return "CL_INVALID_DEVICE";
            case CL_INVALID_CONTEXT:                 return "CL_INVALID_CONTEXT";
            case CL_INVALID_QUEUE_PROPERTIES:        return "CL_INVALID_QUEUE_PROPERTIES";
            case CL_INVALID_COMMAND_QUEUE:           return "CL_INVALID_COMMAND_QUEUE";
            case CL_INVALID_HOST_PTR:                return "CL_INVALID_HOST_PTR";
            case CL_INVALID_MEM_OBJECT:              return "CL_INVALID_MEM_OBJECT";
            case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
            case CL_INVALID_IMAGE_SIZE:              return "CL_INVALID_IMAGE_SIZE";
            case CL_INVALID_SAMPLER:                 return "CL_INVALID_SAMPLER";
            case CL_INVALID_BINARY:                  return "CL_INVALID_BINARY";
            case CL_INVALID_BUILD_OPTIONS:           return "CL_INVALID_BUILD_OPTIONS";
            case CL_INVALID_PROGRAM:                 return "CL_INVALID_PROGRAM";
            case CL_INVALID_PROGRAM_EXECUTABLE:      return "CL_INVALID_PROGRAM_EXECUTABLE";
            case CL_INVALID_KERNEL_NAME:             return "CL_INVALID_KERNEL_NAME";
            case CL_INVALID_KERNEL_DEFINITION:       return "CL_INVALID_KERNEL_DEFINITION";
            case CL_INVALID_KERNEL:                  return "CL_INVALID_KERNEL";
            case CL_INVALID_ARG_INDEX:               return "CL_INVALID_ARG_INDEX";
            case CL_INVALID_ARG_VALUE:               return "CL_INVALID_ARG_VALUE";
            case CL_INVALID_ARG_SIZE:                return "CL_INVALID_ARG_SIZE";
            case CL_INVALID_KERNEL_ARGS:             return "CL_INVALID_KERNEL_ARGS";
            case CL_INVALID_WORK_DIMENSION:          return "CL_INVALID_WORK_DIMENSION";
            case CL_INVALID_WORK_GROUP_SIZE:         return "CL_INVALID_WORK_GROUP_SIZE";
            case CL_INVALID_WORK_ITEM_SIZE:          return "CL_INVALID_WORK_ITEM_SIZE";
            case CL_INVALID_GLOBAL_OFFSET:           return "CL_INVALID_GLOBAL_OFFSET";
            case CL_INVALID_EVENT_WAIT_LIST:         return "CL_INVALID_EVENT_WAIT_LIST";
            case CL_INVALID_EVENT:                   return "CL_INVALID_EVENT";
            case CL_INVALID_OPERATION:               return "CL_INVALID_OPERATION";
            case CL_INVALID_GL_OBJECT:               return "CL_INVALID_GL_OBJECT";
            case CL_INVALID_BUFFER_SIZE:             return "CL_INVALID_BUFFER_SIZE";
            case CL_INVALID_MIP_LEVEL:               return "CL_INVALID_MIP_LEVEL";
            case CL_INVALID_GLOBAL_WORK_SIZE:        return "CL_INVALID_GLOBAL_WORK_SIZE";
        }
        return "CL_UNKNOWN_ERROR";
    }

#if defined(__CL_ENABLE_EXCEPTIONS)
    /*! \brief Handles OpenCL exceptions.
     *
     *  \param err The OpenCL C++ style exception.
     */
    void errHandler(cl::Error err)
    {
        std::cerr << "ERROR: " << err.what() << " " << errString(err.err()) << " (" << err.err() << ")" << std::endl;
    }
#else
    /*! \brief Handles OpenCL error codes.
     *
     *  \param err The OpenCL C style error code.
     *  \param name The name of the function the error occured in.
     */
    void errHandler(cl_int err, STRING_CLASS name)
    {
        if (err == CL_SUCCESS) return;

        std::cerr << "ERROR: " << name << " " << errString(err) << " (" << err << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
#endif // __CL_ENABLE_EXCEPTIONS

    /*! \brief Initializes OpenCL.
     *
     *  \param res_platform A reference to write the cl::Platform to.
     *  \param res_context A reference to write the cl::Context to.
     *  \param res_device A reference to write the cl::Device to.
     */
    void init(cl::Platform& res_platform, cl::Context& res_context, cl::Device& res_device,
              int platform_id = -1, int device_id = -1)
    {
        // Put all available platforms into a vector
        VECTOR_CLASS<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        // Display all available platforms
        if (platforms.size() == 0)
        {
            std::cerr << "No platforms found." << std::endl;
            exit(EXIT_FAILURE);
        }
        if (platform_id == -1) for (unsigned int i = 0; i < platforms.size(); ++i)
        {
            std::cout << i << ". "
                << platforms[i].getInfo<CL_PLATFORM_NAME>() << " "
                << "[" << platforms[i].getInfo<CL_PLATFORM_VERSION>() << "] "
                << platforms[i].getInfo<CL_PLATFORM_VENDOR>() << " "
                << std::endl;
        }
        while (platform_id < 0 || platform_id >= static_cast<int>(platforms.size()))
        {
            std::cout << "Choose a platform: ";
            std::cin >> platform_id;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // Create a context with the specified platform
        cl_context_properties cps[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[platform_id])(), 0 };
        cl::Context context(CL_DEVICE_TYPE_ALL, cps);

        // Put all available devices into a vector
        VECTOR_CLASS<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

        // Display all available devices
        if (devices.size() == 0)
        {
            std::cerr << "No devices found." << std::endl;
            exit(EXIT_FAILURE);
        }
        if (device_id == -1) for (unsigned int i = 0; i < devices.size(); ++i)
        {
            std::cout << i << ". "
                << devices[i].getInfo<CL_DEVICE_NAME>() << " "
                << " [" << devices[i].getInfo<CL_DEVICE_VERSION>() << "] "
                << devices[i].getInfo<CL_DEVICE_VENDOR>() << " "
                << std::endl;
        }
        while (device_id < 0 || device_id >= static_cast<int>(devices.size()))
        {
            std::cout << "Choose a device: ";
            std::cin >> device_id;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // Return results
        res_platform = platforms[platform_id];
        res_context = context;
        res_device = devices[device_id];
    }

    /*! \brief Compiles OpenCL kernels and handles compilation errors.
     *
     *  \param file The location of the OpenCL shader file to compile.
     *  \param name The name of the entrance function defined in the shader file.
     *  \param context A reference the cl::Context that is being used.
     *  \param device A reference the cl::Device that is being used.
     *  \param options Kernel compilation options.
     */
    cl::Kernel* createKernel(STRING_CLASS file, STRING_CLASS name, cl::Context& context, cl::Device& device,
                             STRING_CLASS options = STRING_CLASS())
    {
        std::ifstream ifs(file.c_str());
        STRING_CLASS content(( std::istreambuf_iterator<char>(ifs) ),
                             ( std::istreambuf_iterator<char>()    ));

        cl::Program::Sources source(1, std::make_pair(content.c_str(), content.length() + 1));
        cl::Program program(context, source);

#if defined(__CL_ENABLE_EXCEPTIONS)
        try
        {
            program.build(options.c_str());
        }
        catch (cl::Error& err)
        {
            std::cerr << name << ": " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
            throw err;
        }
#else
        if (CL_SUCCESS != program.build(options.c_str()))
        {
            std::cerr << name << ": " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
            exit(EXIT_FAILURE);
        }
#endif // __CL_ENABLE_EXCEPTIONS

        return new cl::Kernel(program, name.c_str());
    }
}

#endif // CLUT_HPP_