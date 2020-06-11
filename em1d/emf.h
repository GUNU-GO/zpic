/*
 *  emf.h
 *  zpic
 *
 *  Created by Ricardo Fonseca on 10/8/10.
 *  Copyright 2010 Centro de Física dos Plasmas. All rights reserved.
 *
 */

#ifndef __EMF__
#define __EMF__


#include "zpic.h"

#include "current.h"


enum emf_ext_fld { EMF_EXT_FLD_NONE, EMF_EXT_FLD_UNIFORM, EMF_EXT_FLD_CUSTOM };

typedef struct {
	// Type of external field
    enum emf_ext_fld E_type;
    enum emf_ext_fld B_type;
	
    // Uniform external fields
    t_vfld E_0;
	t_vfld B_0;

    // Pointer to custom external E-field function
    t_vfld (*E_custom)(int, float, void*); 
    t_vfld (*B_custom)(int, float, void*); 

    // Pointer to additional data to be passed to the 
    // E_custom and B_custom functions
	void *E_custom_data;
	void *B_custom_data;

	// Fields seen by particules
	t_vfld *E_part_buf;
	t_vfld *B_part_buf;
} t_emf_ext_fld;

enum emf_init_fld { EMF_INIT_FLD_NONE, EMF_INIT_FLD_UNIFORM, EMF_INIT_FLD_CUSTOM };

typedef struct {
	// Type of external field
    enum emf_init_fld E_type;
    enum emf_init_fld B_type;
	
    // Uniform external fields
    t_vfld E_0;
	t_vfld B_0;

    // Pointer to custom external E-field function
    t_vfld (*E_custom)(int, float, void*); 
    t_vfld (*B_custom)(int, float, void*); 

    // Pointer to additional data to be passed to the 
    // E_custom and B_custom functions
	void *E_custom_data;
	void *B_custom_data;

} t_emf_init_fld;

enum emf_diag { EFLD, BFLD, EPART, BPART };
enum emf_boundary { EMF_BC_NONE, EMF_BC_PERIODIC, EMF_BC_OPEN };

typedef struct {

	t_vfld *E;
	t_vfld *B;

	t_vfld *E_buf;
	t_vfld *B_buf;

	// Fields seen by particles
	// When using external fields these will be a combination of the simulation
	// fields and the externally imposed ones. When external fields are off
	// these just point to E and B.
	t_vfld *E_part;
	t_vfld *B_part;

	// Simulation box info
	int nx;
	int gc[2];
	t_fld box;
	t_fld dx;

	// Time step
	float dt;

	// Iteration number
	int iter;

	// Moving window
	int moving_window;
	int n_move;

	// Boundary conditions
	int bc_type;
	t_vfld mur_fld[2], mur_tmp[2];

	// External fields
	t_emf_ext_fld ext_fld;

} t_emf;


// In 1D we only have plane waves
typedef struct {

	float start;	// Front edge of the laser pulse, in simulation units

	float fwhm;		// FWHM of the laser pulse duration, in simulation units
	float rise, flat, fall; // Rise, flat and fall time of the laser pulse, in simulation units

	float a0;		// Normalized peak vector potential of the pulse
	float omega0;	// Laser frequency, normalized to the plasma frequency

	float polarization;

} t_emf_laser;

void emf_get_energy( const t_emf *emf, double energy[] );

void emf_new( t_emf *emf, int nx, t_fld box, const float dt );
void emf_delete( t_emf *emf );
void emf_report( const t_emf *emf, const char field, const char fc );

void emf_add_laser( t_emf* const emf, t_emf_laser* laser );
void emf_init_fld( t_emf* const emf, t_emf_init_fld* init_fld );
void emf_set_ext_fld( t_emf* const emf, t_emf_ext_fld* ext_fld );

void emf_advance( t_emf *emf, const t_current *current );

void emf_move_window( t_emf *emf );
void emf_update_part_fld( t_emf *emf );

void emf_update_gc( t_emf *emf );

double emf_time( void );

void emf_set_moving_window( t_emf* emf );

#endif
