import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {environment} from '../environments/environment';

@Injectable({
  providedIn: 'root'
})
export class ConfigService {
  serverIP!: string;
  serverBaseURl!: string;

  constructor(private http: HttpClient) {
  }

  async init() {
    this.serverIP = await this.http.get('https://johetajava.hu/ip-update/ip/' + environment.ipID + '.txt?q='+Math.random(), {responseType: 'text'}).toPromise();
    this.serverBaseURl = `http://${this.serverIP}:${environment.backendPort}/`;
  }

  static factory(http: HttpClient, configService: ConfigService) {
    return () => configService.init();
  }
}
