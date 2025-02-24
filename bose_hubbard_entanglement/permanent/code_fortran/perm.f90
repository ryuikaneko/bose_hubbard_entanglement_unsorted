module permanent
  implicit none
  integer,parameter :: ip = selected_int_kind(r=15)  !! integer precision
  integer,parameter :: dp = selected_real_kind(p=15) !! double precision
contains
  function cmp_dirty(a,b) result(ret) !! works only for gfortran, use "-fpscomp logicals" for ifort
    integer(ip) :: a,b,ret
!    ret = transfer(a>b,1_ip) - transfer(a<b,1_ip) !! buggy
    ret = transfer(a>b,1) - transfer(a<b,1)
  end function
!
  function cmp(a,b) result(ret)
    integer(ip) :: a,b,ret
    if (a>b) then
      ret = 1_ip
    else if (a<b) then
      ret = -1_ip
    else
      ret = 0_ip
    end if
  end function
!
  function perm(n,M) result(ret)
    integer(ip) :: n
    real(dp), dimension(:,:) :: M
    real(dp) :: ret
    integer(ip) :: i
    integer(ip) :: old_gray
    integer(ip) :: new_gray
    integer(ip) :: gray_diff
    integer(ip) :: gray_diff_index
    integer(ip) :: num_loops
    integer(ip) :: bin_index
    integer(ip) :: sign
    integer(ip) :: direction
    real(dp) :: total
    real(dp), dimension(:), allocatable :: row_comb
    real(dp) :: reduced
    allocate(row_comb(n))
    if (n==0) then
      ret = 0.0_dp
    end if
    row_comb = sum(M,dim=1)
    total = 0.0_dp
    old_gray = 0_ip
    sign = 1_ip
    num_loops = 2_ip ** (n-1_ip)
    do bin_index=1_ip, num_loops
      reduced = product(row_comb)
      total = total + sign * reduced
      new_gray = ieor(bin_index,ishft(bin_index,-1))
      gray_diff = ieor(old_gray,new_gray)
      gray_diff_index = trailz(gray_diff)
!      direction = 2_ip * cmp_dirty(old_gray,new_gray)
      direction = 2_ip * cmp(old_gray,new_gray)
      row_comb = row_comb + M(gray_diff_index+1,:) * direction
      sign = -sign
      old_gray = new_gray
    end do
    ret = total / num_loops
    deallocate(row_comb)
  end function
end module permanent

program main
  use permanent
  implicit none
  integer(ip) :: i,j,n
  real(dp) :: val
  real(dp), dimension(:,:), allocatable :: f
!! prepare matrix
  n = 30
!  n = 4
  allocate( f(n,n) )
  do j = 1,n
    do i = 1,n
      f(i,j) = 1.0_dp
!      f(i,j) = ((i-1_ip)*n+(j-1_ip)+1_ip) * 1.0_dp
    end do
  end do
!! fortran style print
!  do j = 1,n
!    write(*,*) f(:,j)
!  end do
!  write(*,*)
!! c style print
!  do i = 1,n
!    write(*,*) f(i,:)
!  end do
!  write(*,*)
!! calculate permanent
  val = perm(n,f)
  write(*,*) val
!! deallocate matrix
  deallocate(f)
end program main
