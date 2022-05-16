
const sampler_t samplerIn =  CLK_NORMALIZED_COORDS_FALSE | 
                             CLK_ADDRESS_CLAMP_TO_EDGE   | 
                             CLK_FILTER_NEAREST ;


kernel void gradient(write_only image2d_t img, float4 col){
  int2 ipos = (int2)(get_global_id(0), get_global_id(1));
  const int2 imgsize= {get_image_width (img), get_image_height(img)};
  if( ipos.x<0 || ipos.y<0 || ipos.x>=imgsize.x || ipos.y>=imgsize.y ) return; 

  float2 iposf = convert_float2(ipos);
  float2 imgsizef = convert_float2(imgsize);
  float2 st = iposf/imgsizef;
  float4 outcol = (float4)(st.x, 1.0f - st.y, 0.0f, 1.0f);

  write_imagef(img, ipos, outcol);
}

kernel void to_float_rgba( read_only  image2d_t srcImage, write_only image2d_t dstImage){
    const int2 loc = {get_global_id(0), get_global_id(1)};  
    const uint4 src_color = read_imageui(srcImage, samplerIn, loc);
    float4 dst_color = convert_float4(src_color)/255.0f;
    write_imagef(dstImage, loc, dst_color);
}

kernel void to_uint8_rgba( read_only  image2d_t srcImage, write_only image2d_t dstImage){
    const int2 loc = {get_global_id(0), get_global_id(1)};  
    const float4 src_color = read_imagef(srcImage, samplerIn, loc);
    uint4 dst_color = convert_uint4_sat_rte(src_color*255.0f);
    write_imageui(dstImage, loc, dst_color);
}

kernel void render( read_only  image2d_t srcImage, 
                    read_write  image2d_t dstImage,//preImage
                 //   write_only image2d_t dstImage,
                    read_only float16 dest_mat
                    )
{   
    // dest_location check
    const int2 dst_loc = {get_global_id(0), get_global_id(1)};  
    const int2 dst_size= {get_image_width (dstImage), get_image_height(dstImage)};
    if( dst_loc.x<0 || dst_loc.y<0 || dst_loc.x>=dst_size.x || dst_loc.y>=dst_size.y) return; 

    // src_location from scale matrix
    const float* dest_mat_ptr = &dest_mat;
    const float4 src_loc_cal = { 
                        dest_mat_ptr[0] * dst_loc.x + dest_mat_ptr[4] * dst_loc.y + dest_mat_ptr[8]  * 0.0f + dest_mat_ptr[12] * 1.0f,
                        dest_mat_ptr[1] * dst_loc.x + dest_mat_ptr[5] * dst_loc.y + dest_mat_ptr[9]  * 0.0f + dest_mat_ptr[13] * 1.0f,
                        dest_mat_ptr[2] * dst_loc.x + dest_mat_ptr[6] * dst_loc.y + dest_mat_ptr[10] * 0.0f + dest_mat_ptr[14] * 1.0f,
                        dest_mat_ptr[3] * dst_loc.x + dest_mat_ptr[7] * dst_loc.y + dest_mat_ptr[11] * 0.0f + dest_mat_ptr[15] * 1.0f
                       };    
    
    const float4 src_loc = src_loc_cal/src_loc_cal.w;
    const int2 src_size = {get_image_width (srcImage), get_image_height(srcImage)};
    if( src_loc.x<0 || src_loc.y<0 || src_loc.x>=src_size.x || src_loc.y>=src_size.y ) return;
    
    const float4 next_color = read_imagef(srcImage, samplerIn, src_loc.xy);
    const float4 pre_color  = (float4){1.0f, 1.0f, 1.0f,0.1f};//read_imagef(dstImage, samplerIn, dst_loc);
 
    float4 color = mix(next_color, pre_color,1.0f - next_color.w);
    write_imagef(dstImage, dst_loc, color);
}


float2 cal_distance(const float2 p1, const float2 p2, const float2 p3){
   float m1 = p1.x * p2.y + p2.x * p3.y + p3.x * p1.y;
   float m2 = p2.x * p1.y + p3.x * p2.y + p1.x * p3.y;
   float area = fabs(m1 - m2);
   float2 ret = {};
   do {
        if (area == 0.0) break;
        float2 a_vec = p2 - p1;
        float2 d_vec = { a_vec.y, -a_vec.x };
        float d_area = a_vec.x * a_vec.x + a_vec.y * a_vec.y;
        float ratio = d_area / area;
        ret = d_vec / ratio;
    } while (false);

    return ret;
}

kernel void draw_lines(
            read_only private  float2    start,
            read_only private  float2    end,
            read_only private  float     r,
            read_only private  float4    color,
            read_only private  float16   src_mat,
            read_only  image2d_t preImage,
            write_only image2d_t dstImage
   ) {
    const int2 src_loc = {get_global_id(0), get_global_id(1)};
    const int2 dst_size= {get_image_width (dstImage), get_image_height(dstImage)};
    
    const float* mat = &src_mat;
    const float4 dest_loc_cal = { 
                        mat[0] * src_loc.x + mat[4] * src_loc.y + mat[8] * 0.0f + mat[12] * 1.0f,
                        mat[1] * src_loc.x + mat[5] * src_loc.y + mat[9] * 0.0f + mat[13] * 1.0f,
                        mat[2] * src_loc.x + mat[6] * src_loc.y + mat[10] * 0.0f+ mat[14] * 1.0f,
                        mat[3] * src_loc.x + mat[7] * src_loc.y + mat[11] * 0.0f+ mat[15] * 1.0f
                        };
    float2 f_loc = {dest_loc_cal.x/dest_loc_cal.w, dest_loc_cal.y/dest_loc_cal.w};

    for(int x = -1; x <=1 ;++x){
        for(int y= -1; y <=1 ;++y){
            float2 of_pt =  { f_loc.x + x * 0.5f, f_loc.y +  y * 0.5f};
            int2 dest_loc = { round(of_pt.x), round(of_pt.y) };
            if (dest_loc.x < 0 || dest_loc.y < 0 || dest_loc.x >= dst_size.x || dest_loc.y >= dst_size.y) continue;
           
            float2 distance = cal_distance(start, end, (float2){dest_loc.x, dest_loc.y});            
            float len = sqrt(distance.x*distance.x+distance.y*distance.y);
            if(len>r) len = r;
            float ratio = len/r;

            float4 pre_color = read_imagef(preImage, samplerIn, dest_loc);
            float4 mix_color = mix(color, pre_color, ratio);
            write_imagef(dstImage, dest_loc, mix_color);
        }
    }

}