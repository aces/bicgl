 
#include  <internal_volume_io.h>
#include  <gs_specific.h>

private  void  make_matrix(
    Transform  *trans,
    Matrix     gl_trans )
{
    int      i, j;

    for_less( i, 0, 4 )
    {
        for_less( j, 0, 4 )
        {
            gl_trans[i][j] = Transform_elem( *trans, j, i );
        }
    }
}

public  void  GS_mult_transform(
    Transform  *transform )
{
    Matrix  gl_trans;

    make_matrix( transform, gl_trans );

    multmatrix( gl_trans );
}


public  void  GS_load_transform(
    Transform  *transform )
{
    Matrix  gl_trans;

    make_matrix( transform, gl_trans );

    loadmatrix( gl_trans );
}

public  void  GS_get_transform(
    Transform  *trans )
{
    int      i, j;
    Matrix   gl_trans;

    getmatrix( gl_trans );

    for_less( i, 0, 4 )
    {
        for_less( j, 0, 4 )
        {
            Transform_elem( *trans, j, i ) = gl_trans[i][j];
        }
    }
}

public  void  GS_ortho(
    Real   x_min,
    Real   x_max,
    Real   y_min,
    Real   y_max,
    Real   z_min,
    Real   z_max )
{
    ortho( x_min, x_max, y_min, y_max, z_min, z_max );
}

public  void  GS_ortho_2d(
    int   x_min,
    int   x_max,
    int   y_min,
    int   y_max )
{
    ortho( (Real) x_min - 0.5, (Real) x_max + 0.5,
           (Real) y_min - 0.5, (Real) y_max + 0.5, -1.0, 1.0 );
}

public  void  GS_frustum(
    Real   x_min,
    Real   x_max,
    Real   y_min,
    Real   y_max,
    Real   z_min,
    Real   z_max )
{
    window( x_min, x_max, y_min, y_max, z_min, z_max );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : initialize_window_view
@INPUT      : window
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Initializes the window view, called on creation of the window.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

/* ARGSUSED */

public  void  GS_initialize_window_view(
    Gwindow   window )
{
}

public  void  GS_push_transform()
{
    pushmatrix();
}

public  void  GS_pop_transform()
{
    popmatrix();
}

public  void  GS_set_viewport(
    int            x_min,
    int            x_max,
    int            y_min,
    int            y_max )
{
    viewport( x_min, x_max, y_min, y_max );
}

public  void  clear_overlay_planes()
{
#ifndef  TWO_D_ONLY
    Matrix          save_projection;

    if( G_has_overlay_planes() )
    {
        mmode( MPROJECTION );
        getmatrix( save_projection );
        mmode( MVIEWING );

        pushmatrix();
        pushviewport();

        fullscrn();
/*
        drawmode( NORMALDRAW );
        blendfunction( BF_ONE, BF_ZERO );
*/
        drawmode( OVERDRAW );
        color( 0 );
        clear();
        endfullscrn();

        restore_bitplanes( (Gwindow) NULL );

        popviewport();
        popmatrix();

        mmode( MPROJECTION );
        loadmatrix( save_projection );
        mmode( MVIEWING );
    }
#endif
}

public  void  GS_set_matrix_mode(
    Matrix_modes   mode )
{
    int  gl_mode;

    switch( mode )
    {
    case VIEWING_MATRIX:
        gl_mode = MVIEWING;
        break;
    case PROJECTION_MATRIX:
        gl_mode = MPROJECTION;
        break;
    }

    mmode( gl_mode );
}
