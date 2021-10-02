import {Injectable} from '@angular/core';
import {HttpEvent, HttpHandler, HttpInterceptor, HttpRequest} from '@angular/common/http';
import {TempService} from './temp.service';
import {Observable, throwError} from 'rxjs';
import {catchError} from 'rxjs/operators';
import {AlertService} from './alert.service';

@Injectable()
export class ErrorInterceptor implements HttpInterceptor {
  constructor(private alertService: AlertService) {
  }
  intercept(request: HttpRequest<any>, next: HttpHandler): Observable<HttpEvent<any>> {
    return next.handle(request).pipe(catchError(err => {
      if (err.status === 401) {
        // auto logout if 401 response returned from api
        TempService.logout();
        location.reload();
      }

      this.alertService.alert({
        title: "Error",
        message: err.message,
        showOkButton: false,
        showCloseButton: true,
        showCancelButton: false,
        icon: 'error'
      });
      return throwError(err);
    }))
  }
}
