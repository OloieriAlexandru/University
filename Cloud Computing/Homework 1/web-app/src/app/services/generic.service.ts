import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root',
})
export class GenericService {
  constructor(private http: HttpClient) {}

  public get<T>(api: string, url: string): Observable<T> {
    return this.http.get<T>(api + url, { headers: this.buildHeaders() });
  }

  public post<T>(api: string, url: string, data?: any): Observable<T> {
    return this.http.post<T>(api + url, data, { headers: this.buildHeaders() });
  }

  private buildHeaders(): HttpHeaders {
    return new HttpHeaders({ 'Content-Type': 'application/json' });
  }
}
