import {Injectable} from '@angular/core';
import {Router, CanActivate, ActivatedRouteSnapshot, RouterStateSnapshot} from '@angular/router';
import {TempService} from './temp.service';


@Injectable({providedIn: 'root'})
export class AuthGuard implements CanActivate {
  constructor(private router: Router) {
  }

  canActivate(route: ActivatedRouteSnapshot, state: RouterStateSnapshot) {
    if (TempService.isLoggedIn()) {
      return true;
    }

    // not logged in so redirect to login page with the return url
    this.router.navigate(['/login'], {queryParams: {returnUrl: state.url}});
    return false;
  }
}
