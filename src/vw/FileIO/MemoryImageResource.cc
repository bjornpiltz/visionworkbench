// __BEGIN_LICENSE__
//  Copyright (c) 2006-2013, United States Government as represented by the
//  Administrator of the National Aeronautics and Space Administration. All
//  rights reserved.
//
//  The NASA Vision Workbench is licensed under the Apache License,
//  Version 2.0 (the "License"); you may not use this file except in
//  compliance with the License. You may obtain a copy of the License at
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// __END_LICENSE__


#include <vw/config.h>
#include <vw/FileIO/MemoryImageResource.h>

#if defined(VW_HAVE_PKG_JPEG) && VW_HAVE_PKG_JPEG==1
#  include <vw/FileIO/MemoryImageResourceJPEG.h>
#endif

#if defined(VW_HAVE_PKG_PNG) && VW_HAVE_PKG_PNG==1
#  include <vw/FileIO/MemoryImageResourcePNG.h>
#endif

#if defined(VW_HAVE_PKG_GDAL) && VW_HAVE_PKG_GDAL==1
#  include <vw/FileIO/MemoryImageResourceGDAL.h>
#endif

#if defined(VW_HAVE_PKG_OPENEXR) && VW_HAVE_PKG_OPENEXR==1
#  include <vw/FileIO/MemoryImageResourceOpenEXR.h>
#endif

#include <map>

#include <boost/assign/list_of.hpp>
#include <boost/lambda/construct.hpp>
#include <boost/function.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace {
   std::string clean_type(const std::string& type) {
     return boost::to_lower_copy(boost::trim_left_copy_if(type, boost::is_any_of(".")));
   }
}

namespace vw {

  SrcMemoryImageResource* SrcMemoryImageResource::open( const std::string& type, const uint8* data, size_t len ) {
    boost::shared_array<const uint8> p(data, NOP());
    return SrcMemoryImageResource::open(type, p, len);
  }

  SrcMemoryImageResource* SrcMemoryImageResource::open( const std::string& ext, boost::shared_array<const uint8> data, size_t len ) {
      const std::string type = clean_type(ext);
#if defined(VW_HAVE_PKG_JPEG) && VW_HAVE_PKG_JPEG==1
    if(type=="jpg" || type=="jpeg" || type=="image/jpeg")
      return new SrcMemoryImageResourceJPEG(data, len);
#endif
#if defined(VW_HAVE_PKG_PNG) && VW_HAVE_PKG_PNG==1
    if(type=="png" || type=="image/png")
      return new SrcMemoryImageResourcePNG(data, len);
#endif
#if defined(VW_HAVE_PKG_GDAL) && VW_HAVE_PKG_GDAL==1
    if (type=="tif" || type=="tiff" || type=="image/tiff")
        return new SrcMemoryImageResourceGDAL(data, len);
#endif
#if defined(VW_HAVE_PKG_OPENEXR) && VW_HAVE_PKG_OPENEXR==1
    if (type=="exr" || type=="image/exr")
        return new SrcMemoryImageResourceEXR(data, len);
#endif
    vw_throw(NoImplErr() << "Unsupported file format: " << type);
  }

  DstMemoryImageResource* DstMemoryImageResource::create( const std::string& ext, const ImageFormat& format ) {
    const std::string type = clean_type(ext);
#if defined(VW_HAVE_PKG_JPEG) && VW_HAVE_PKG_JPEG==1
    if (type=="jpg" || type=="jpeg" || type=="image/jpeg")
        return new DstMemoryImageResourceJPEG(format);
#endif
#if defined(VW_HAVE_PKG_PNG) && VW_HAVE_PKG_PNG==1
    if (type=="png" || type=="image/png")
        return new DstMemoryImageResourcePNG(format);
#endif
#if defined(VW_HAVE_PKG_GDAL) && VW_HAVE_PKG_GDAL==1
    if (type=="tif" || type=="tiff" || type=="image/tiff")
        return new DstMemoryImageResourceGDAL(format);
#endif
#if defined(VW_HAVE_PKG_OPENEXR) && VW_HAVE_PKG_OPENEXR==1
    if (type=="exr" || type=="image/exr")
        return new DstMemoryImageResourceEXR(format);
#endif
    vw_throw(NoImplErr() << "Unsupported file format: " << type);
  }

} // namespace vw
