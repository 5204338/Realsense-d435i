#include <librealsense2/rs.hpp>
#include <iostream>

int main() {
    // Declare RealSense pipeline, pipe profile, and align object
    rs2::pipeline pipe;
    rs2::config cfg;
    cfg.enable_stream(rs2_stream::RS2_STREAM_DEPTH);
    cfg.enable_stream(rs2_stream::RS2_STREAM_COLOR);
    rs2::pipeline_profile profile = pipe.start(cfg);
    rs2::align align_to_color(RS2_STREAM_COLOR);

    while (true) {
        // Wait for a coherent pair of frames: depth and color
        rs2::frameset frameset = pipe.wait_for_frames();
        rs2::frameset aligned_frameset = align_to_color.process(frameset);

        // Get the aligned depth frame
        rs2::video_frame depth_frame = aligned_frameset.get_depth_frame();

        // Get the depth intrinsics after alignment
        rs2::video_stream_profile depth_stream = depth_frame.get_profile().as<rs2::video_stream_profile>();
        rs2_intrinsics depth_intrinsics = depth_stream.get_intrinsics();

        // Print the depth intrinsics
        std::cout << "Depth Intrinsics:\n";
        std::cout << "Width: " << depth_intrinsics.width << std::endl;
        std::cout << "Height: " << depth_intrinsics.height << std::endl;
        std::cout << "Principal Point (cx, cy): (" << depth_intrinsics.ppx << ", " << depth_intrinsics.ppy << ")" << std::endl;
        std::cout << "Focal Length (fx, fy): (" << depth_intrinsics.fx << ", " << depth_intrinsics.fy << ")" << std::endl;
        std::cout << "Distortion Model: " << depth_intrinsics.model << std::endl;
        std::cout << "Coefficients: ";
        for (int i = 0; i < 5; i++) {
            std::cout << depth_intrinsics.coeffs[i] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}