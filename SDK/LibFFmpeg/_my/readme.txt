//libx264.c
#if defined(_MSC_VER)
//#define X264_API_IMPORTS 1
#endif

//libx265.c
#if defined(_MSC_VER)
//#define X265_API_IMPORTS 1
#endif

//libaomdec.c

    struct aom_codec_dec_cfg deccfg = {
        .threads = FFMIN(avctx->thread_count ? avctx->thread_count : av_cpu_count(), 16),
        .w=0,
        .h=0,
        .allow_lowbitdepth=1
    };



2022 5.7