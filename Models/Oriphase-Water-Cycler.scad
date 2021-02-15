$fn                 = 400   ;

plate               =  2.0  ;

havingPumpMuzzle    = true  ;
havingWaterLevel    = false ;
havingWaterLevelHat = false ;
havingWaterShell    = false ;

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
  } ;
}

module oriphase_water_cycler_pump_muzzle ()
{
  outer_diameter =  11.0 ;
  outer_height   = 20.0 ;
  inner_height   = 12.0 ;
  inner_d1       =  8.0 ;
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
  difference ( ) {
    union ( ) {
    } ;
    union ( ) {
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
  difference ( ) {
    union ( ) {
    } ;
    union ( ) {
    } ;
  } ;
}
