#include  <def_graphics.h>

int  main(
    int   argc,
    char  *argv[] )
{
    Status         status;
    int            n_alloced, x_size, y_size, i, sizes[MAX_DIMENSIONS];
    int            n_slices_displayed;
    Real           intensity, separations[MAX_DIMENSIONS];
    Real           min_value, max_value;
    pixels_struct  pixels;
    Volume         volume;
    window_struct  *window;
    Real           scale;
    Colour         *rgb_map;
    char           *filename;
    static String  dim_names[] = { MIxspace, MIyspace, MIzspace };

    initialize_argument_processing( argc, argv );
    (void) get_string_argument( "/nil/david/big_data/sphere.fre", &filename );
    (void) get_int_argument( 1, &n_slices_displayed );

    status = input_volume( filename, dim_names, FALSE, &volume );

    get_volume_voxel_range( volume, &min_value, &max_value );

    print( "%g %g   %g %g\n", min_value, max_value,
           CONVERT_VOXEL_TO_VALUE( volume, min_value ),
           CONVERT_VOXEL_TO_VALUE( volume, max_value ) );

    if( status != OK )
        return( 1 );

    get_volume_sizes( volume, sizes );
    get_volume_separations( volume, separations );
    print( "Volume %s: %d by %d by %d\n",
            filename, sizes[X], sizes[Y], sizes[Z] );
    print( "Thickness: %g %g %g\n",
            separations[X], separations[Y], separations[Z] );

    status = G_create_window( "Volume Browser", -1, -1, -1, -1, &window );

    G_get_window_size( window, &x_size, &y_size );

    n_alloced = 0;

    get_volume_voxel_range( volume, &min_value, &max_value );
    ALLOC( rgb_map, (int)max_value+1 );

    for_less( i, 0, (int) max_value+1 )
    {
        intensity = (Real) i / max_value;
        rgb_map[i] = make_Colour_0_1( intensity, intensity, intensity );
    }

    scale = (Real) x_size / ((Real) sizes[X] * separations[X]);
    if( (Real) y_size / ((Real) sizes[Y] * separations[Y]) < scale )
        scale = (Real) y_size / ((Real) sizes[Y] * separations[Y]);

    create_volume_slice( BOX_FILTER, (Real) n_slices_displayed,
                         volume, (Real) (sizes[Z] - 1) / 2.0,
                         0.0, 0.0, scale, scale,
                         (Volume) NULL, 0.0, 0.0, 0.0, 0.0, 0.0,
                         X, Y, Z, x_size, y_size, RGB_PIXEL, FALSE,
                         (unsigned short **) NULL,
                         &rgb_map, &n_alloced, &pixels );

    G_set_view_type( window, PIXEL_VIEW );
    G_draw_pixels( window, &pixels );
    G_update_window( window );

    print( "Hit return: " );
    (void) getchar();

    return( status != OK );
}
