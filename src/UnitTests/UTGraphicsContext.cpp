#include "gtest/gtest.h"

#include "GraphicsContextGLFW.hpp"

/// \brief Test if the context initialized correctly
TEST( UTGraphicsContextGLFW, ContextInitialize )
{

  GraphicsContextGLFW context;

  ASSERT_TRUE( context.initialize() );

  ASSERT_TRUE( context.isInitialized() );

}

/// \brief Make sure the finalizing of the context works as expected
TEST( UTGraphicsContextGLFW, ContextFinalize )
{

  GraphicsContextGLFW context;

  ASSERT_TRUE( context.initialize() );

  ASSERT_TRUE( context.finalize() );

  ASSERT_FALSE( context.isInitialized() );

  ASSERT_FALSE( context.finalize() );

}
