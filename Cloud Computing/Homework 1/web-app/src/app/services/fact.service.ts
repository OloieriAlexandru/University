import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';

import { environment } from 'src/environments/environment';

import { GenericService } from './generic.service';

import { RandomImageResponse } from '../models/RandomImageResponse';
import { RandomFactResponse } from '../models/RandomFactResponse';
import { RandomFactImageRequest } from '../models/RandomFactImageRequest';
import { RandomFactImageResponse } from '../models/RandomFactImageResponse';
import { RandomFactRequest } from '../models/RandomFactRequest';

@Injectable({
  providedIn: 'root',
})
export class FactService {
  private URL = environment.factImageGeneratorURL + '/api/v1/';

  constructor(private http: GenericService) {}

  public getRandomImage(): Observable<RandomImageResponse> {
    return this.http.get(this.URL, 'random-image');
  }

  public getRandomFact(
    request: RandomFactRequest
  ): Observable<RandomFactResponse> {
    return this.http.post(this.URL, 'random-fact', request);
  }

  public buildFactImage(
    request: RandomFactImageRequest
  ): Observable<RandomFactImageResponse> {
    return this.http.post(this.URL, 'fact-image', request);
  }
}
