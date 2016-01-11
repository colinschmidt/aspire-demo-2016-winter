
    __kernel void fn1(global float* It, global float* Ix, global float* Iy, global float* denom, global const float* im0, global const float* im1) {
        int index = get_global_id(0);
        if (index < 307200) {
            float _hm_generated_1;
			float _hm_generated_2;
			float _hm_generated_0;

			It[index] = im1[index] - im0[index];
			
			{
			int x = index % 640;
			int y = index / 640;
			float accum = 0.0;
			
			accum += -0.0833333333333f * im1[max(y + -2, 0) * 640 + x];
			
			accum += -0.666666666667f * im1[max(y + -1, 0) * 640 + x];
			
			accum += 0.666666666667f * im1[min(y + 1, 480 - 1) * 640 + x];
			
			accum += 0.0833333333333f * im1[min(y + 2, 480 - 1) * 640 + x];
			
			Iy[index] = accum;
			}
			
			{
			int x = index % 640;
			int y = index / 640;
			float accum = 0.0;
			
			accum += -0.0833333333333f * im1[y * 640 + max(x + -2, 0)];
			
			accum += -0.666666666667f * im1[y * 640 + max(x + -1, 0)];
			
			accum += 0.666666666667f * im1[y * 640 + min(x + 1, 640 - 1)];
			
			accum += 0.0833333333333f * im1[y * 640 + min(x + 2, 640 - 1)];
			
			Ix[index] = accum;
			}
			_hm_generated_1 = pow(Iy[index], 2);
			_hm_generated_2 = pow(Ix[index], 2);
			_hm_generated_0 = _hm_generated_2 + _hm_generated_1;
			denom[index] = _hm_generated_0 + 225.0;

        }
    }
        
    __kernel void fn2(global const float* It, global const float* Ix, global const float* Iy, global const float* denom, global const float* u, global float* u_new, global const float* v, global float* v_new) {
        int index = get_global_id(0);
        if (index < 307200) {
            float t;
			float ubar;
			float vbar;
			float _hm_generated_7;
			float _hm_generated_4;
			float _hm_generated_6;
			float _hm_generated_8;
			float _hm_generated_5;
			float _hm_generated_3;

			
			{
			int x = index % 640;
			int y = index / 640;
			float accum = 0.0;
			
			accum += 0.0833333333333f * u[max(y + -1, 0) * 640 + max(x + -1, 0)];
			
			accum += 0.166666666667f * u[max(y + -1, 0) * 640 + x];
			
			accum += 0.0833333333333f * u[max(y + -1, 0) * 640 + min(x + 1, 640 - 1)];
			
			accum += 0.166666666667f * u[y * 640 + max(x + -1, 0)];
			
			accum += 0.166666666667f * u[y * 640 + min(x + 1, 640 - 1)];
			
			accum += 0.0833333333333f * u[min(y + 1, 480 - 1) * 640 + max(x + -1, 0)];
			
			accum += 0.166666666667f * u[min(y + 1, 480 - 1) * 640 + x];
			
			accum += 0.0833333333333f * u[min(y + 1, 480 - 1) * 640 + min(x + 1, 640 - 1)];
			
			ubar = accum;
			}
			
			{
			int x = index % 640;
			int y = index / 640;
			float accum = 0.0;
			
			accum += 0.0833333333333f * v[max(y + -1, 0) * 640 + max(x + -1, 0)];
			
			accum += 0.166666666667f * v[max(y + -1, 0) * 640 + x];
			
			accum += 0.0833333333333f * v[max(y + -1, 0) * 640 + min(x + 1, 640 - 1)];
			
			accum += 0.166666666667f * v[y * 640 + max(x + -1, 0)];
			
			accum += 0.166666666667f * v[y * 640 + min(x + 1, 640 - 1)];
			
			accum += 0.0833333333333f * v[min(y + 1, 480 - 1) * 640 + max(x + -1, 0)];
			
			accum += 0.166666666667f * v[min(y + 1, 480 - 1) * 640 + x];
			
			accum += 0.0833333333333f * v[min(y + 1, 480 - 1) * 640 + min(x + 1, 640 - 1)];
			
			vbar = accum;
			}
			_hm_generated_5 = Iy[index] * vbar;
			_hm_generated_6 = Ix[index] * ubar;
			_hm_generated_4 = _hm_generated_6 + _hm_generated_5;
			_hm_generated_3 = _hm_generated_4 + It[index];
			t = _hm_generated_3 / denom[index];
			_hm_generated_7 = Ix[index] * t;
			u_new[index] = ubar - _hm_generated_7;
			_hm_generated_8 = Iy[index] * t;
			v_new[index] = vbar - _hm_generated_8;

        }
    }
        