$fn                  = 400   ;

plate                = 2.0   ;
gap                  = 3.0   ;

havingPumpMuzzle     = false ;
havingWaterLevel     = false ;
havingWaterLevelHat  = false ;
havingWaterShell     = false ;
havingTankFrontRight = false ;
havingTankFrontLeft  = true  ;

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
  hole_radius = 2.2 ;
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
        cylinder ( d = hole_radius , h = 20.0 ) ;
      } ;
    } ;
  } ;
}
