import {Injectable} from '@angular/core';
import {StoredInterval} from './interval';
import {HttpClient} from '@angular/common/http';
import {ConfigService} from './config.service';

@Injectable({
  providedIn: 'root'
})
export class TempService {
  targetTemp = 0;
  currentTemp = 0;
  isHeating = true;

  constructor(private http: HttpClient, private config: ConfigService) {
    this.getTargetTemp().then(t => {
      this.targetTemp = t;
    });

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

  async getCurrentTemp(): Promise<number> {
    let temp = await this.http.get(`${this.config.serverBaseURl}get-temp`, {responseType: 'text'}).toPromise();
    return Number(temp);
  }

  setNormalTemp(temp: number): void {
    this.http.get(`${this.config.serverBaseURl}set-normal-temp?temp=${temp}`, {responseType: 'text'}).toPromise()
      .catch(e => {
        // todo error handling
      });
  }

  async getNormalTemp(): Promise<number> {
    let temp = await this.http.get(`${this.config.serverBaseURl}get-normal-temp`, {responseType: 'text'}).toPromise();
    return Number(temp);
  }

  getIntervalsStoredData(): Promise<{ items: StoredInterval[], normalTemp: number }> {
    return this.http.get(`${this.config.serverBaseURl}get-intervals`).toPromise() as Promise<any>;
  }

  async getTargetTemp(): Promise<number> {
    let temp = await this.http.get(`${this.config.serverBaseURl}get-target-temp`, {responseType: 'text'}).toPromise();
    return Number(temp);
  }

  async getIsHeating(): Promise<boolean> {
    let temp = await this.http.get(`${this.config.serverBaseURl}get-is-heating`, {responseType: 'text'}).toPromise();
    return temp == '1';
  }

  async deleteInterval(order: number): Promise<any>{
    return this.http.get(`${this.config.serverBaseURl}remove-interval?order=${order}`, {responseType: 'text'}).toPromise();
  }

  resetEverything() {
    return this.http.get(`${this.config.serverBaseURl}reset`, {responseType: 'text'}).toPromise();
  }

  resetIntervals() {
    return this.http.get(`${this.config.serverBaseURl}reset-intervals`, {responseType: 'text'}).toPromise();
  }

  restart() {
    return this.http.get(`${this.config.serverBaseURl}restart`, {responseType: 'text'}).toPromise();
  }

  modifyProperty(param: string, value: string | number, order: number){
    return this.http.get(`${this.config.serverBaseURl}modify-property?param=${param}&value=${value}&index=${Number(order)}`, {responseType: 'text'}).toPromise();
  }
}
