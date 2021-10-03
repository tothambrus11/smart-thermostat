import {Injectable} from '@angular/core';
import {StoredInterval} from './interval';
import {HttpClient, HttpHeaders} from '@angular/common/http';
import {ConfigService} from './config.service';

@Injectable({
  providedIn: 'root'
})
export class TempService {
  targetTemp = 0;
  currentTemp = 0;
  isHeating = true;

  constructor(private http: HttpClient, private config: ConfigService) {
    if (TempService.isLoggedIn()) {
      this.getTargetTemp().then(t => {
        this.targetTemp = t;
      });
    }

    if (window.EventSource) {
      this.startEventListening();
    } else {
      this.startHttpTempUpdateInterval();
    }
  }

  startEventListening() {
    let eventSource = new EventSource(`${this.config.serverBaseURl}events`);

    eventSource.addEventListener('temperature', (e: any) => {
      this.currentTemp = Number(e.data);
    });

    eventSource.addEventListener('target_temperature', (e: any) => {
      this.targetTemp = Number(e.data);
    });

    eventSource.addEventListener('is_heating', (e: any) => {
      this.isHeating = e.data == '1';
    });

    eventSource.addEventListener('open', e => {
      console.log('Events Connected');
    });

    eventSource.addEventListener('error', (e: any) => {
      if (e.target.readyState !== EventSource.OPEN) {
        console.log('Events Disconnected');
      }
    }, false);
  }

  startHttpTempUpdateInterval() {
    setInterval(async () => {
      this.getCurrentTemp()
        .then(res => {
          this.currentTemp = Number(res);
        })
        .catch(err => {
          console.error(err);
        });
    }, 1000);
  }

  get authData() {
    return {
      username: localStorage.getItem('username'),
      password: localStorage.getItem('password'),
    };
  }

  get username() {
    return localStorage.getItem('username');
  }

  get password() {
    return localStorage.getItem('password');
  }

  getActiveInterval() {
    return this.getWithCredentials(`get-active-interval`);
  }

  async getCurrentTemp(): Promise<number> {
    let temp = await this.getWithCredentials(`get-temp`, {responseType: 'text'});
    return Number(temp);
  }

  setNormalTemp(temp: number): void {
    this.getWithCredentials(`set-normal-temp?temp=${temp}`, {responseType: 'text'});
  }

  async getNormalTemp(): Promise<number> {
    let temp = await this.getWithCredentials(`get-normal-temp`, {responseType: 'text'});
    return Number(temp);
  }

  getIntervalsStoredData(): Promise<{ items: StoredInterval[], normalTemp: number }> {
    return this.getWithCredentials(`get-intervals`) as Promise<any>;
  }

  async getTargetTemp(): Promise<number> {
    let temp = await this.getWithCredentials(`get-target-temp`, {responseType: 'text'});
    return Number(temp);
  }

  async getIsHeating(): Promise<boolean> {
    let temp = await this.getWithCredentials(`get-is-heating`, {responseType: 'text'});
    return temp == '1';
  }

  async deleteInterval(order: number): Promise<any> {
    return this.getWithCredentials(`remove-interval?order=${order}`, {responseType: 'text'});
  }

  resetEverything() {
    return this.getWithCredentials(`reset`, {responseType: 'text'});
  }

  resetIntervals() {
    return this.getWithCredentials(`reset-intervals`, {responseType: 'text'});
  }

  restart() {
    return this.getWithCredentials(`restart`, {responseType: 'text'});
  }

  getNetworkData() {
    return this.getWithCredentials('get-network-data');
  }

  getApCredentials() {
    return this.getWithCredentials('get-ap-credentials');
  }

  swapIntervals(order1: number, order2: number) {
    return this.getWithCredentials(`change-interval-order?order1=${order1}&order2=${order2}`, {responseType: 'text'});
  }

  modifyProperty(param: string, value: string | number, order: number) {
    return this.getWithCredentials(`modify-property?param=${param}&value=${value}&index=${Number(order)}`, {responseType: 'text'});
  }

  newInterval() {
    return this.getWithCredentials('new-interval', {responseType: 'text'});
  }

  static isLoggedIn() {
    return localStorage.getItem('username') && localStorage.getItem('password');
  }

  static logout() {
    localStorage.removeItem('username');
    localStorage.removeItem('password');
  }

  getWithCredentials(route: string, options_: any = {}, postData: any = {}) {
    let body = new URLSearchParams();
    body.set('username', localStorage.getItem('username') || '');
    body.set('password', localStorage.getItem('password') || '');

    for (let key in postData) {
      body.set(key, postData[key]);
    }

    let options = {
      headers: new HttpHeaders().set('Content-Type', 'application/x-www-form-urlencoded'),
      ...options_
    };

    return this.http.post(this.config.serverBaseURl + route, body.toString(), options).toPromise() as Promise<any>;
  }

  setWifiCredentials(ssid: string, password: string) {
    return this.getWithCredentials('set-wifi-credentials', {}, {wifiSSID: ssid, wifiPassword: password});
  }
}
