#shader compute
#version 430
layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform writeonly image2D img_output;

void main() {
	
	// base pixel colour for image
	vec4 pixel = vec4(0.0, 0.0, 0.0, 1.0);
	// get index in global work group i.e x,y position
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

	//
	// interesting stuff happens here later
	//
	pixel = vec4(1.0, 0.0, 0.4, 1.0);

	// output to a specific pixel in the image
	imageStore(img_output, pixel_coords, pixel);
	
}

