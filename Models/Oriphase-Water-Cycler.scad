$fn                  = 400   ;

plate                = 2.0   ;
gap                  = 3.0   ;

havingPumpMuzzle     = false ;
havingWaterLevel     = false ;
havingWaterLevelHat  = false ;
havingWaterShell     = false ;
havingTankFrontRight = false ;
havingTankFrontLeft  = false ;
havingBackPanel      = true  ;

translate ( [ 0 , 0 , 0 ] )
{
  union ( ) {
    if ( havingPumpMuzzle ) {
      translate ( [ 0 , 0 , 0 ] ) {
        oriphase_water_cycler_pump_muzzle ( ) ;
      } ;
    } ;
    if ( havingWaterLevel ) {
      translate ( [ 0 , 0 , 0 ] ) {
        oriphase_water_cycler_water_level ( ) ;
      } ;
    } ;
    if ( havingWaterLevelHat ) {
      translate ( [ 0 , 0 , 0 ] ) {
        oriphase_water_cycler_water_level_hat ( ) ;
      } ;
    } ;
    if ( havingWaterShell ) {
      translate ( [ 0 , 0 , 0 ] ) {
        oriphase_water_cycler_water_shell ( ) ;
      } ;
    } ;
    if ( havingTankFrontRight ) {
      translate ( [ 0 , 0 , 0 ] ) {
        oriphase_water_cycler_tank_front_right ( ) ;
      } ;
    } ;
    if ( havingTankFrontLeft ) {
      translate ( [ 0 , 0 , 0 ] ) {
        mirror ( [ 1 , 0 , 0 ] ) {
          oriphase_water_cycler_tank_front_right ( ) ;
        } ;
      } ;
    } ;
    if ( havingBackPanel ) {
      translate ( [ 0 , 0 , 0 ] ) {
        oriphase_water_cycler_back_panel ( ) ;
      } ;
    } ;
  } ;
}

module oriphase_water_cycler_pump_muzzle ()
{
  outer_diameter = 12.0 ;
  outer_height   = 20.0 ;
  inner_height   = 12.0 ;
  inner_d1       =  8.5 ;
  inner_d2       =  6.5 ;
  hole_radius    =  6.0 ;
  hole_height    =  8.0 ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cylinder ( d = outer_diameter , h = outer_height ) ;
      } ;
    } ;
    union ( ) {
      translate ( [ 0.0 , 0.0 , -0.1 ] ) {
        cylinder ( d1 = inner_d1 , d2 = inner_d2 , h = inner_height + 0.1 ) ;
      } ;
      translate ( [ 0.0 , 0.0 , inner_height - 0.1 ] ) {
        cylinder ( d = hole_radius , h = hole_height + 10.0 ) ;
      } ;
    } ;
  } ;
}

module oriphase_water_cycler_water_level ()
{
  width  = 20.4 ;
  length = 15.0 ;
  height = 35.0 ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube ( size = [ width + ( plate * 2 ) , length , height ] ) ;
      } ;
    } ;
    union ( ) {
      translate ( [ plate , plate + 1.0 , -10.0 ] ) {
        cube ( size = [ width , 1.5 , height + 20.0 ] ) ;
      } ;
      translate ( [ plate , plate , plate ] ) {
        cube ( size = [ width , length - ( plate * 2 ) , height ] ) ;
      } ;
    } ;
  } ;
}

module oriphase_water_cycler_water_level_hat ()
{
  difference ( ) {
    union ( ) {
    } ;
    union ( ) {
    } ;
  } ;
}

module oriphase_water_cycler_water_shell ()
{
  width       =  25.0 ;
  length      =  15.0 ;
  height      =  35.0 ;
  bwidth      =  40.0 ;
  bheight     = 115.0 ;
  hole_radius =   6.0 ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube ( size = [ bwidth , length , bheight ] ) ;
      } ;
    } ;
    union ( ) {
      translate ( [ plate , plate , -10.0 ] ) {
        cube ( size = [ width , length , bheight + 20.0 ] ) ;
      } ;
      translate ( [ plate , -10 , 80.0 ] ) {
        cube ( size = [ width , length + 20.0 , bheight ] ) ;
      } ;
      translate ( [ width + ( plate * 2 ) , plate , -0.1 ] ) {
        cube ( size = [ width , length , 15.1 ] ) ;
      } ;
      translate ( [ width + ( plate * 2 ) , plate , 25.0 ] ) {
        cube ( size = [ width , length , 80.0 ] ) ;
      } ;
      translate ( [ width + ( plate * 2 ) + ( bwidth - width - ( plate * 2 ) ) / 2.0 , ( ( length - plate ) / 2.0 ) + plate , -10.0 ] ) {
        cylinder ( d = hole_radius , h = bheight + 20.0 ) ;
      } ;
    } ;
  } ;
}

module oriphase_water_cycler_tank_front_right ( )
{
  width       = 11.0 ;
  base        =  6.0 ;
  jwidth      = 30.0 ;
  fence       = 10.0 ;
  jlength     = ( plate * 2 ) + gap ;
  length      = 20.0 + base + jlength ;
  hole_radius = 2.5 ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube ( size = [ width , length , plate ] ) ;
      } ;
      translate ( [ width - 0.1 , base , 0.0 ] ) {
        cube ( size = [ jwidth + 1.1 , jlength , plate ] ) ;
      } ;
      translate ( [ width + 1.0 , base , 0.0 ] ) {
        cube ( size = [ jwidth , jlength , fence ] ) ;
      } ;
    } ;
    union ( ) {
      translate ( [ width , base + plate , plate ] ) {
        cube ( size = [ jwidth + 2.0 , gap , fence ] ) ;
      } ;
      translate ( [ 5.0 , length - 10.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
    } ;
  } ;
}

module oriphase_water_cycler_back_panel ( )
{
  width       = 146.0 ;
  length      =  96.0 ;
  height      =  12.0 ;
  side        =  10.0 ;
  bwidth      =  width  + ( side * 2 ) ;
  blength     =  length + side ;
  jgap        = ( plate * 2 ) + gap ;
  hole_radius =  2.5 ;
  switch_w    =  13.0 ;
  switch_h    =  20.0 ;
  dig_base    = -10.0 ;
  dig_height  =  50.0 ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube ( size = [ bwidth , blength , plate ] ) ;
      } ;
      translate ( [ side , side , plate - 0.1 ] ) {
        cube ( size = [ width , length , height ] ) ;
      } ;
    } ;
    union ( ) {
      translate ( [ side + jgap , side + jgap , plate ] ) {
        cube ( size = [ width - ( jgap * 2 ) , length - ( jgap * 2 ) , height + 10.0 ] ) ;
      } ;
      translate ( [ side + plate , side + plate , plate ] ) {
        cube ( size = [ width - ( plate * 2 ) , gap , height + 10.0 ] ) ;
      } ;
      translate ( [ side + plate , length + side - plate - gap , plate ] ) {
        cube ( size = [ width - ( plate * 2 ) , gap , height + 10.0 ] ) ;
      } ;
      translate ( [ side + plate , side + plate , plate ] ) {
        cube ( size = [ gap , length - ( plate * 2 ) , height + 10.0 ] ) ;
      } ;
      translate ( [ width + side - plate - gap , side + plate , plate ] ) {
        cube ( size = [ gap , length - ( plate * 2 ) , height + 10.0 ] ) ;
      } ;
      translate ( [ 80.0 , 40.0 , dig_base ] ) {
        mirror ( [ 1 , 0 , 0 ] ) {
          oriphase_water_cycler_power_jack ( dig_height ) ;
        } ;
      } ;
      translate ( [ 110.0 , 44.0 , dig_base ] ) {
        cube ( size = [ switch_w , switch_h , dig_height ] ) ;
      } ;
      translate ( [ 5.0 , 30.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ 5.0 , 90.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ width + side + 5.0 , 30.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ width + side + 5.0 , 90.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ 40.0 , 5.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ bwidth - 40.0 , 5.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
    } ;
  } ;
}

module oriphase_water_cycler_power_jack ( hh )
{
  cornerRadius      =  2.5 ;
  power_width       = 48.0 ;
  power_height      = 28.5 ;
  power_corner      =  5.0 ;
  power_shift       =  4.0 ;
  power_hole        = 40.0 ;
  power_hole_gap    = ( power_hole - power_height ) / 2.0 ;
  hole_radius       =  1.6 ;
  union ( ) {
    translate ( [ cornerRadius , cornerRadius , 0 ] ) {
      roundedBox ( power_width - power_corner , power_height , hh , cornerRadius ) ;
    } ;
    translate ( [ power_width - power_corner - power_shift , 0 , 0 ] ) {
      linear_extrude ( height = hh ) {
        polygon ( points = [
            [                          0 ,                           0 ] ,
            [ power_shift                ,                           0 ] ,
            [ power_shift + power_corner ,                power_corner ] ,
            [ power_shift + power_corner , power_height - power_corner ] ,
            [ power_shift                , power_height                ] ,
            [ 0                          , power_height                ]
          ] ,
          convexity = 1 ) ;
      } ;
    } ;
    translate ( [ power_width / 2 , - power_hole_gap , 0.5 ] ) {
      cylinder ( r = hole_radius , h = hh ) ;
    } ;
    translate ( [ power_width / 2 , power_height + power_hole_gap , 0.5 ] ) {
      cylinder ( r = hole_radius , h = hh ) ;
    } ;
  } ;
}

module roundedBox ( width , length , height , radius )
{
  dRadius = 2 * radius ;
  minkowski ( ) {
    cube     ( size = [ width - dRadius , length - dRadius , height ] ) ;
    cylinder ( d = dRadius , h = 0.01 ) ;
  }
}
