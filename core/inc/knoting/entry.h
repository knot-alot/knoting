#pragma once
#include <bx/filepath.h>
#include <bx/spscqueue.h>
#ifndef ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR
#define ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR 1
#endif  // ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR

namespace entry {
bx::AllocatorI* getDefaultAllocator();
bx::FileReaderI* getFileReader();
bx::AllocatorI* getAllocator();

};
  
