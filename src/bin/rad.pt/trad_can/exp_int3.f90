!****************************************************************************************
!                               FUNCTION EXP_INT3
!****************************************************************************************
! Calculates the 3rd exponential integral from "Numerical Recipes in Fortran 90"
! Press, Teukolsky et al

DOUBLE PRECISION function exp_int3(tau)

USE parameters
implicit none
REAL (KIND=DBL), INTENT (IN) :: tau
REAL (KIND=DBL) :: b, c, d, h, a, del, eps, big, fact, psi
INTEGER :: l_, k_



   ! Set some function parameters
   ! 1. Desired relative error. Not smaller than machine precision.
        eps = EPSILON (tau)
   ! 2. Number near the largest representable floating-point number
        big = HUGE (tau) * eps


   if (tau < 0d0) then
       ! Optical depth less than zero
       write (*,*) 'Negative Optical depth in exp_int3. Program stopped.'
       stop
   else if (tau == 0d0) then
       ! Special case
       exp_int3 = 1d0 / (THIRD - 1)
   else if (tau > 1d0) then
       ! Uses Lentz's algorithm
       b = tau + THIRD
       c = big
       d = 1d0 / b

      if (abs(b) < TOL_ZERO) then
         write (*,*) 'zero error in ' , 'expint 1'
         stop
      end if


       h = d
       do k_ = 1, MAX_ITER
           a = - REAL(k_ * (THIRD - 1 + k_))
           b = b + 2d0
           d = 1d0 / ( a * d + b)

      if (abs(a* d + b) < TOL_ZERO) then
         write (*,*) 'zero error in ' , 'expint 2'
         stop
      end if

      if (abs(c) < TOL_ZERO) then
         write (*,*) 'zero error in ' , 'expint 3'
         stop
      end if


           c = b + a / c
           del = c * d
           h = h * del
           if (ABS(del - 1d0) <= eps) EXIT
       end do
       if (k_ > MAX_ITER) then           ! Safety condition
           write (*,*) 'exp_int3: continued fraction failed'
           stop
       end if
       exp_int3 = h * EXP(- tau)
   else
       ! Convergence is too slow for 0 < tau < 1 .
       ! Use series representation instead.
       exp_int3 = 1d0 / real(THIRD-1)
       fact = 1d0
       do k_ = 1, MAX_ITER
           fact = - fact * tau / REAL(k_)
           if (k_ /= THIRD-1) then
               del = - fact / real(k_ - (THIRD - 1))
           else
               ! Compute psi
               psi = - EULER
               do l_ = 1, THIRD-1
                   psi = psi + 1d0 / REAL(l_)
               end do
               del = fact * ( - LOG(tau) + psi)
           end if
           exp_int3 = exp_int3 + del
           if (ABS(del) < ABS(exp_int3) * eps) EXIT
       end do
       if (k_ > MAX_ITER) then        ! Safety condition
           write (*,*) 'exp_int3: series failed'
           stop
       end if
   end if


   return

end function exp_int3

