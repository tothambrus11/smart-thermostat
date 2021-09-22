import {Injectable} from '@angular/core';
import {HttpEvent, HttpHandler, HttpInterceptor, HttpRequest} from '@angular/common/http';
import {Observable} from 'rxjs';
import {TempService} from './temp.service';
import {ConfigService} from './config.service';

@Injectable()
export class BasicAuthInterceptor implements HttpInterceptor {
  constructor(private configService: ConfigService) {
  }

  intercept(request: HttpRequest<any>, next: HttpHandler): Observable<HttpEvent<any>> {
    // add header with basic auth credentials if user is logged in and request is to the api url
    /*const isApiUrl = request.url.startsWith(this.configService.serverBaseURl);
    console.log('interceptiing 1');
    if (TempService.isLoggedIn() && isApiUrl) {
      console.log('cloning');

      request = request.clone({
        setHeaders: {
          'Authorization': `Basic ${window.btoa('admin:admin')}`
        }
      });
      console.log(request)
    }
*/
    return next.handle(request);
  }
}
