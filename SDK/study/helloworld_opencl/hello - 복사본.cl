// www.jocl.org/samples/JOCLSimpleImage.java
//

const sampler_t samplerIn =  CLK_NORMALIZED_COORDS_FALSE | 
                             CLK_ADDRESS_CLAMP           | 
                             CLK_FILTER_LINEAR;

__kernel void render( __read_only  image2d_t srcImage, 
                      __read_only  image2d_t preImage,
                      __write_only image2d_t dstImage, 
                      __global __read_only  float* mat
                      )
{
    int2 dst_loc = {get_global_id(0), get_global_id(1)};
    int2 dst_size ={get_image_width (dstImage), get_image_width (dstImage)};
    if(dst_loc.x<0 || dst_loc.y<0 || dst_loc.x>=dst_size.x || dst_loc.y>=dst_size.y) return;


   // src_loc_cal = (dst_loc*mat), src_loc= src_loc_cal/src_loc_cal.w
    float4 src_loc_cal = { mat[0] * dst_loc.x + mat[4] * dst_loc.y + mat[8] * 0.0f + mat[12] * 1.0f,
                       mat[1] * dst_loc.x + mat[5] * dst_loc.y + mat[9] * 0.0f + mat[13] * 1.0f,
                       mat[2] * dst_loc.x + mat[6] * dst_loc.y + mat[10] * 0.0f+ mat[14] * 1.0f,
                       mat[3] * dst_loc.x + mat[7] * dst_loc.y + mat[11] * 0.0f+ mat[15] * 1.0f
                       };
    float2 src_loc ={src_loc_cal.x/src_loc_cal.w, src_loc_cal.y/src_loc_cal.w};
    int2 src_size = {get_image_width (srcImage), get_image_height(srcImage)};
    if(src_loc.x<0 || src_loc.y<0 || src_loc.x>=src_size.x || src_loc.y>=src_size.y) return;
        
    //
    
    uint4 next_color    = read_imageui(srcImage, samplerIn, src_loc);
    uint4 pre_color     = read_imageui(preImage, samplerIn, dst_loc); 
        
    float a0 = (float)next_color.w/255.0f;
    float b0 = (float)pre_color.w/255.0f;
    float t0 = a0 + b0 * (1.0f - a0);
    float r1 = a0 / t0;
    float r2 = b0 * (1.0f - a0)/t0;

    uint4 result_color; 
    result_color.w = convert_uchar4(t0 * 255.0f);
    result_color.x = convert_uchar4((float)next_color.x * r1 + (float)pre_color.x * r2);
    result_color.y = convert_uchar4((float)next_color.y * r1 + (float)pre_color.y * r2);
    result_color.z = convert_uchar4((float)next_color.z * r1 + (float)pre_color.z * r2);
           
    write_imageui(dstImage, dst_loc, result_color);
   // if(current_color.w==255) printf("%f ", fcolor.w);
}