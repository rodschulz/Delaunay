ffmpeg -framerate 3 -i ./output/img_%d.png -c:v libx264 -r 30 -pix_fmt yuv420p ./output/video.mp4
