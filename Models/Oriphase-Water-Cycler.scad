$fn                   = 400   ;

plate                 = 2.0   ;
gap                   = 3.0   ;

havingPumpMuzzle      = false ;
havingWaterLevel      = false ;
havingWaterLevelHat   = false ;
havingWaterShell      = false ;
havingTankFrontRight  = false ;
havingTankFrontLeft   = false ;
havingBackPanel       = false ;
havingTBone           = false ;
havingRightAngle      = false ;
havingRightScrew      = false ;
havingRightWaterProof = false ;
havingLeftWaterProof  = false ;
havingPumpWaterProof  = false ;
havingShieldLatcher   = false ;
havingPipeHolder      = false ;
havingFrontPlate      = false ;
havingWaterNozzle     = false ;
havingNozzleClamp     = true  ;

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
    if ( havingTBone ) {
      translate ( [ 0 , 0 , 0 ] ) {
        oriphase_water_cycler_t_bone ( 15.0 , 12.0 ) ;
      } ;
    } ;
    if ( havingRightAngle ) {
      translate ( [ 0 , 0 , 0 ] ) {
        RightAngleConnector ( 1.6 , 3.3 , 18.0 , 12.0 , 72.0 ) ;
      } ;
    } ;
    if ( havingRightScrew ) {
      translate ( [ 0 , 0 , 0 ] ) {
        RightAngleScrew ( 1.6 , 3.3 , 18.0 , 12.0 , 60.0 , 4.4 , 2.1 , 3.8 ) ;
      } ;
    } ;
    if ( havingRightWaterProof ) {
      translate ( [ 0 , 0 , 0 ] ) {
        mirror ( [ 1 , 0 , 0 ] ) {
          LeftWaterProof ( ) ;
        } ;
      } ;
    } ;
    if ( havingLeftWaterProof ) {
      translate ( [ 0 , 0 , 0 ] ) {
        LeftWaterProof ( ) ;
      } ;
    } ;
    if ( havingPumpWaterProof ) {
      translate ( [ 0 , 0 , 0 ] ) {
        PumpWaterProof ( ) ;
      } ;
    } ;
    if ( havingShieldLatcher ) {
      translate ( [ 0 , 0 , 0 ] ) {
        WaterShieldLatcher ( 60.0 ) ;
      } ;
    }
    if ( havingPipeHolder ) {
      translate ( [ 0 , 0 , 0 ] ) {
        WaterPipeHolder ( 10.0 ) ;
      } ;
    }
    if ( havingFrontPlate ) {
      translate ( [ 0 , 0 , 0 ] ) {
        WaterFrontPlate ( ) ;
      } ;
    }
    if ( havingWaterNozzle ) {
      translate ( [ 0 , 0 , 0 ] ) {
        WaterNozzle ( ) ;
      } ;
    }
    if ( havingNozzleClamp ) {
      translate ( [ 0 , 0 , 0 ] ) {
        WaterNozzleClamp ( ) ;
      } ;
    }
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
  width       = 14.0 ;
  base        =  6.0 ;
  jwidth      = 30.0 ;
  fence       = 15.0 ;
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
      translate ( [ 6.5 , length - 10.0 , -10.0 ] ) {
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
  side        =  13.0 ;
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
//        cube ( size = [ switch_w , switch_h , dig_height ] ) ;
      } ;
      translate ( [ ( side / 2.0 ) , 30.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ ( side / 2.0 ) , 90.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ width + side + ( side / 2.0 ) , 30.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ width + side + ( side / 2.0 ) , 90.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ 40.0 , ( side / 2.0 ) , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ bwidth - 40.0 , ( side / 2.0 ) , -10.0 ] ) {
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

module oriphase_water_cycler_t_bone ( mounter , height )
{
  bwidth = gap + ( plate * 2 ) ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube ( size = [ bwidth , bwidth + ( mounter * 2 ) , height + plate ] ) ;
      } ;
      translate ( [ bwidth - 0.1 , mounter , 0.0 ] ) {
        cube ( size = [ mounter + 0.1 , bwidth , height + plate ] ) ;
      } ;
    } ;
    union ( ) {
      translate ( [ plate , -10.0 , plate ] ) {
        cube ( size = [ gap , bwidth + ( mounter * 2 ) + 20.0 , height + plate ] ) ;
      } ;
      translate ( [ plate , mounter + plate , plate ] ) {
        cube ( size = [ bwidth + mounter , gap , height + plate ] ) ;
      } ;
    } ;
  }
}

module hexagon_screw ( r , h ) {
  w = r / 2.0 ;
  f = 0.8660254 * r ;
  linear_extrude ( height = h ) {
    polygon ( points = [
                [  r ,  0 ] ,
                [  w ,  f ] ,
                [ -w ,  f ] ,
                [ -r ,  0 ] ,
                [ -w , -f ] ,
                [  w , -f ] ,
              ] ,
              convexity = 1 ) ;
  } ;
}

module RightAngleConnector ( thickness , acrylic , vertical , horizontal , length )
{
  dt = ( thickness * 2 ) ;
  dg = - 1.0 ;
  gw = - ( dg * 2 ) ;
  w  = dt + acrylic ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        difference ( ) {
          translate ( [ 0.0 , 0.0 , 0.0 ] ) {
            cube ( size = [ w , length , vertical ] ) ;
          } ;
          translate ( [ thickness , dg , thickness ] ) {
            cube ( size = [ acrylic , length + gw , vertical ] ) ;
          } ;
        } ;
      } ;
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        difference ( ) {
          translate ( [ w - 0.4 , 0.0 , 0.0 ] ) {
            cube ( size = [ horizontal + 0.4 , length , w ] ) ;
          } ;
          translate ( [ w , dg , thickness ] ) {
            cube ( size = [ horizontal + gw , length + gw , acrylic ] ) ;
          } ;
        } ;
      } ;
    } ;
  } ;
}

module RightAngleScrew ( thickness , acrylic , vertical , horizontal , length , screw_trap , screw_hole , bottom_hole )
{
  dt = ( thickness * 2 ) ;
  w  = dt + acrylic ;
  sh =  4.4 ;
  sw = 12.0 ;
  sc = ( sw / 2.0 ) ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        RightAngleConnector ( thickness , acrylic , vertical , horizontal , length ) ;
      } ;
      translate ( [ w - 0.4 , 0.0  , w - 0.4 ] ) {
        cube     ( size = [ horizontal + 0.4 , sw , sh ] ) ;
      } ;
      translate ( [ w - 0.4 , length - sw , w - 0.4 ] ) {
        cube     ( size = [ horizontal + 0.4 , sw , sh ] ) ;
      } ;
      translate ( [ w - 0.4 , sw - 0.4 , w - 0.4 ] ) {
        rotate ( a = 90 , v = [ 0 , 0 , 1 ] ) {
          rotate ( a = 90 , v = [ 1 , 0 , 0 ] ) {
            linear_extrude ( height = horizontal + 0.4 ) {
              polygon ( points = [
                  [ 0.0 , 0.0 ] ,
                  [  sh , 0.0 ] ,
                  [  sh , 0.4 ] ,
                  [ 0.4 ,  sh ] ,
                  [ 0.0 ,  sh ] ,
                  [ 0.0 , 0.0 ] ,
                ] ,
                convexity = 1 ) ;
            } ;
          } ;
        } ;
      } ;
      translate ( [ horizontal + w , length - sw , w - 0.4 ] ) {
        rotate ( a = 270 , v = [ 0 , 0 , 1 ] ) {
          rotate ( a = 90 , v = [ 1 , 0 , 0 ] ) {
            linear_extrude ( height = horizontal + 0.4 ) {
              polygon ( points = [
                  [ 0.0 , 0.0 ] ,
                  [  sh , 0.0 ] ,
                  [  sh , 0.4 ] ,
                  [ 0.4 ,  sh ] ,
                  [ 0.0 ,  sh ] ,
                  [ 0.0 , 0.0 ] ,
                ] ,
                convexity = 1 ) ;
            } ;
          } ;
        } ;
      } ;
    } ;
    union ( ) {
      translate ( [ w + ( horizontal / 2.0 ) , sc  , -1.0 ] ) {
        union ( ) {
          translate ( [ 0.0 , 0.0 , w + sh - 2.5 ] ) {
            hexagon_screw ( screw_trap , sh ) ;
          } ;
          translate ( [ 0.0 , 0.0 , 0.0 ] ) {
            cylinder ( r = screw_hole , h = w + ( sh * 2 ) ) ;
          } ;
          translate ( [ 0.0 , 0.0 , 0.0 ] ) {
            cylinder ( r = bottom_hole , h = thickness + acrylic ) ;
          } ;
        } ;
      } ;
      translate ( [ w + ( horizontal / 2.0 ) , length - sc  , -1.0 ] ) {
        union ( ) {
          translate ( [ 0.0 , 0.0 , w + sh - 2.5 ] ) {
            hexagon_screw ( screw_trap , sh ) ;
          } ;
          translate ( [ 0.0 , 0.0 , 0.0 ] ) {
            cylinder ( r = screw_hole , h = w + ( sh * 2 ) ) ;
          } ;
          translate ( [ 0.0 , 0.0 , 0.0 ] ) {
            cylinder ( r = bottom_hole , h = thickness + acrylic ) ;
          } ;
        } ;
      } ;
    } ;
  } ;
}

module LeftWaterProof ( )
{
  bwidth  = gap + ( plate * 2 ) ;
  width   = 10.0 ;
  length  = 60.0 ;
  jwidth  = width + bwidth ;
  jlength = 28.0 ;
  blength = jlength + bwidth ;
  gshift  =  5.0 ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube     ( size = [ jwidth , length , plate ] ) ;
      } ;
      translate ( [ 0.0 , blength , - width + 0.1 ] ) {
        cube     ( size = [ plate , length - blength , width ] ) ;
      } ;
      translate ( [ 0.0 , jlength , plate - 0.1 ] ) {
        cube     ( size = [ jwidth , bwidth , width ] ) ;
      } ;
    } ;
    union ( ) {
      translate ( [ -10.0 , jlength + plate , plate ] ) {
        cube     ( size = [ jwidth + 20.0 , gap , width ] ) ;
      } ;
      translate ( [ gshift , jlength + plate , -10.0 ] ) {
        cube     ( size = [ jwidth + 20.0 , gap , width + 20.0 ] ) ;
      } ;
    } ;
  } ;
}

module PumpWaterProof ( )
{
  swidth  =  20.0 ;
  jlength =  80.0 ;
  jwidth  =  50.0 ;
  blength = 120.0 ;
  xlength =  30.0 ;
  bheight =  20.0 ;
  bwidth  = ( plate * 2 ) + swidth + jwidth ;
  cheight =   5.0 ;
  dhole   =   5.0 ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube     ( size = [ bwidth , blength , plate ] ) ;
      } ;
      translate ( [ swidth , 0.0 , plate - 0.1 ] ) {
        difference ( ) {
          translate ( [ 0.0 , 0.0 , 0.0 ] ) {
            cube     ( size = [ jwidth + ( plate * 2 ) , blength , bheight ] ) ;
          } ;
          translate ( [ plate , plate , -10.0 ] ) {
            cube     ( size = [ jwidth , blength - ( plate * 2 ) , bheight + 20.0 ] ) ;
          } ;
        } ;
      } ;
      translate ( [ swidth + plate , plate + xlength , plate - 0.1 ] ) {
        cube     ( size = [ jwidth + 0.2 , plate , cheight ] ) ;
      } ;
      translate ( [ swidth + plate , plate + xlength + jlength , plate - 0.1 ] ) {
        cube     ( size = [ jwidth + 0.2 , plate , cheight ] ) ;
      } ;
    } ;
    union ( ) {
      translate ( [ bwidth - 25.0 , blength - 10.0 , 10.0 ] ) {
        cube     ( size = [ 15.0 , 30.0 , bheight ] ) ;
      } ;
      translate ( [ 10.0 , 55.0 , -10.0 ] ) {
        cylinder ( d = dhole , h = 20.0 ) ;
      } ;
      translate ( [ 10.0 , 90.0 , -10.0 ] ) {
        cylinder ( d = dhole , h = 20.0 ) ;
      } ;
    } ;
  } ;
}

module WaterShieldLatcher ( length )
{
  plank = 13.0 ;
  clamp =  8.0 ;
  hole  =  5.0 ;
  side  = 12.0 ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube     ( size = [ length , plank + clamp + plate  , plate ] ) ;
      } ;
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube     ( size = [ length , clamp + plate , gap + ( plate * 2 ) ] ) ;
      } ;
    } ;
    union ( ) {
      translate ( [ -10.0 , -10.0 , plate ] ) {
        cube     ( size = [ length + 20.0 , clamp + 10.0 , gap ] ) ;
      } ;
      translate ( [ side , clamp + plate + ( plank / 2.0 ) , -10.0 ] ) {
        cylinder ( d = hole , h = 20.0 ) ;
      } ;
      translate ( [ length - side , clamp + plate + ( plank / 2.0 ) , -10.0 ] ) {
        cylinder ( d = hole , h = 20.0 ) ;
      } ;
    } ;
  } ;
}

module WaterPipeHolder ( length )
{
  width  = 24.0 ;
  height = 15.0 ;
  throat = 10.0 ;
  hole   =  6.0 ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube     ( size = [ width + ( plate * 2 ) , height + ( plate * 2 ) , length ] ) ;
      } ;
      translate ( [ plate + ( ( width - throat ) / 2 ) , height + ( plate * 2 ) , 0.0 ] ) {
        cube     ( size = [ throat , hole + plate , length ] ) ;
      } ;
    } ;
    union ( ) {
      translate ( [ plate , plate , -10.0 ] ) {
        cube     ( size = [ width , height , length + 20.0 ] ) ;
      } ;
      translate ( [ plate + 5.0 , -10.0 , -10.0 ] ) {
        cube     ( size = [ width - 10.0 , height , length + 20.0 ] ) ;
      } ;
      translate ( [ plate + ( width / 2 ) - 2.0 , height + ( plate * 2 ) + hole - 2.0 , -10.0 ] ) {
        cube     ( size = [ 4.0 , height , length + 20.0 ] ) ;
      } ;
      translate ( [ plate + ( width / 2 ) , height + ( plate * 2 ) + ( hole / 2 ) , -10.0 ] ) {
        cylinder ( d = hole , h = length + 20.0 ) ;
      } ;
    } ;
  } ;
}

module WaterFrontPlate ( )
{
  width       = 146.0 ;
  length      =  70.0 ;
  height      =   6.0 ;
  side        =  13.0 ;
  bwidth      =  width  + ( side * 2 ) ;
  blength     =  length + side ;
  jgap        = ( plate * 2 ) + gap ;
  hole_radius =   2.5 ;
  switch_w    =  13.0 ;
  switch_h    =  20.0 ;
  dig_base    = -10.0 ;
  dig_height  =  50.0 ;
  margin      =   5.0 ;
  twidth      =  60.0 ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube ( size = [ bwidth , blength , plate ] ) ;
      } ;
      difference ( ) {
        translate ( [ side , side , plate - 0.1 ] ) {
          cube ( size = [ width , length , height ] ) ;
        } ;
        translate ( [ side + plate , side + plate , plate ] ) {
          cube ( size = [ width - ( plate * 2 ) , length - ( plate * 2 ) , height + 10.0 ] ) ;
        } ;
      } ;
      difference ( ) {
        translate ( [ 136.5 , 60.0 , plate - 0.1 ] ) {
          cylinder ( d = 7.0 , h = 6.0 ) ;
        } ;
        translate ( [ 136.5 , 60.0 , plate ] ) {
          cylinder ( d = 5.5 , h = 6.0 ) ;
        } ;
      } ;
      difference ( ) {
        translate ( [ 106.5 , 60.0 , plate - 0.1 ] ) {
          cylinder ( d = 7.0 , h = 6.0 ) ;
        } ;
        translate ( [ 106.5 , 60.0 , plate ] ) {
          cylinder ( d = 5.5 , h = 6.0 ) ;
        } ;
      } ;
    } ;
    union ( ) {
      translate ( [ side + plate + margin , side + plate + margin , -10.0 ] ) {
        cube ( size = [ twidth , length - ( plate * 2 ) - ( margin * 2 ) , height + 20.0 ] ) ;
      } ;
      translate ( [ 130.0 , 25.0 , dig_base ] ) {
        cube ( size = [ switch_w , switch_h , dig_height ] ) ;
      } ;
      translate ( [ 136.5 , 60.0 , -10.0 ] ) {
        cylinder ( d = 3.0 , h = 20.0 ) ;
      } ;
      translate ( [ 100.0 , 25.0 , dig_base ] ) {
        cube ( size = [ switch_w , switch_h , dig_height ] ) ;
      } ;
      translate ( [ 106.5 , 60.0 , -10.0 ] ) {
        cylinder ( d = 3.0 , h = 20.0 ) ;
      } ;
      translate ( [ ( side / 2.0 ) , 30.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ ( side / 2.0 ) , 70.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ width + side + ( side / 2.0 ) , 30.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ width + side + ( side / 2.0 ) , 70.0 , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ 40.0 , ( side / 2.0 ) , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
      translate ( [ bwidth - 40.0 , ( side / 2.0 ) , -10.0 ] ) {
        cylinder ( r = hole_radius , h = 20.0 ) ;
      } ;
    } ;
  } ;
}

module WaterNozzleClamp ( )
{
  width  = 24.0 ;
  height = 15.0 ;
  length = 20.0 ;
  roller = 25.0 ;
  hole   =  6.0 ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube     ( size = [ width + ( plate * 2 ) , height + ( plate * 2 ) , length ] ) ;
      } ;
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube     ( size = [ plate , height + ( plate * 2 ) + roller , length ] ) ;
      } ;
      translate ( [ width + plate , 0.0 , 0.0 ] ) {
        cube     ( size = [ plate , height + ( plate * 2 ) + roller , length ] ) ;
      } ;
    } ;
    union ( ) {
      translate ( [ plate , plate , plate ] ) {
        cube     ( size = [ width , height , length + 20.0 ] ) ;
      } ;
      translate ( [ plate + 5.0 , 10.0 , plate ] ) {
        cube     ( size = [ width - 10.0 , height , length + 20.0 ] ) ;
      } ;
      translate ( [ -10.0 , height + ( plate * 2 ) + roller - 6.0 , length / 2.0 ] ) {
        rotate ( a = 90 , v = [ 0 , 1 , 0 ] ) {
          cylinder ( d = 2.0 , h = width + 40.0 ) ;
        } ;
      } ;
      translate ( [ -10.0 , ( height / 2.0 ) + plate , length / 2.0 ] ) {
        rotate ( a = 90 , v = [ 0 , 1 , 0 ] ) {
          cylinder ( d = 4.0 , h = width + 40.0 ) ;
        } ;
      } ;
    } ;
  } ;
}

module WaterNozzle ( )
{
  width  = 24.0 ;
  length = 10.0 ;
  nozzle = 10.0 ;
  hole   =  6.0 ;
  difference ( ) {
    union ( ) {
      translate ( [ 0.0 , 0.0 , 0.0 ] ) {
        cube     ( size = [ width , length , length ] ) ;
      } ;
      translate ( [ width / 2.0 , length / 2.0 , length - 0.1 ] ) {
        cylinder ( d = hole + ( plate * 2 ) , h = nozzle + 0.1 ) ;
      } ;
    } ;
    union ( ) {
      translate ( [ -15.0 , length / 2.0 , length / 2.0 ] ) {
        rotate ( a = 90 , v = [ 0 , 1 , 0 ] ) {
          cylinder ( d = 2.0 , h = 20.0 ) ;
        } ;
      } ;
      translate ( [ width - 5.0 , length / 2.0 , length / 2.0 ] ) {
        rotate ( a = 90 , v = [ 0 , 1 , 0 ] ) {
          cylinder ( d = 2.0 , h = 20.0 ) ;
        } ;
      } ;
      translate ( [ width / 2.0 , length / 2.0 , -10.0 ] ) {
        cylinder ( d = hole , h = length + nozzle + 20.0 ) ;
      } ;
    } ;
  } ;
}
