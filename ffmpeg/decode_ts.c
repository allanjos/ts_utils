#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "%s <transport stream file path>\n", argv[0]);

        return -1;
    }

    avdevice_register_all(); /* for device & add libavdevice/avdevice.h headerfile*/
    avcodec_register_all();
    av_register_all();
    
    const char *url = argv[1];

    AVFormatContext *format_context = NULL;

    int ret = avformat_open_input(&format_context, url, NULL, NULL);

    if (ret < 0) {
        fprintf(stderr, "Cannot open input.\n");
        exit(1);
    }

    printf("format name = %s\n", format_context->iformat->name);
    printf("format long name = %s\n", format_context->iformat->long_name);
    printf("streams: %d\n", format_context->nb_streams);


	if ((ret = avformat_find_stream_info(format_context, 0)) < 0) {
		printf( "Failed to retrieve input stream information");
		//goto end;
	}

    printf("\n");

    int videoindex;

	for (int i=0; i < format_context->nb_streams; i++) {
        printf("STREAM %d:\n", i);

        if (format_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoindex = i;

            printf("video stream index = %d\n");
		}

        AVStream *in_stream = format_context->streams[i];

        printf("frames: %ld\n", in_stream->nb_frames);
        printf("stream id: %d (0x%x)\n", in_stream->id, in_stream->id);

	    av_dump_format(format_context, i, url, 0);
    }

    printf("HEADER:\n");

    printf("FRAMES:\n");

    long packet_count = 0;

    while (1) {
        AVPacket packet;

        //Get an AVPacket

        ret = av_read_frame(format_context, &packet);

        if (ret < 0)
            break;

        printf("packet %ld\n", packet_count++);

        printf("\tpts = %ld\n", packet.pts);
        printf("\tdts = %ld\n", packet.dts);
        printf("\tsize = %ld\n", packet.size);
        printf("\tstream_index = %ld\n", packet.stream_index);
        printf("\tduration = %ld\n", packet.duration);
        printf("\tpos = %ld\n", packet.pos);
        printf("\tconvergence_duration = %ld\n", packet.convergence_duration);

		AVStream *in_stream  = format_context->streams[packet.stream_index];

        printf("\tstream id: %ld\n", in_stream->id);


		av_free_packet(&packet);
    }

    avformat_close_input(&format_context);
    
}
