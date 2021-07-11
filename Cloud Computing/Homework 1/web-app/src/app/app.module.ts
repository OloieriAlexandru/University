import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { HttpClientModule } from '@angular/common/http';
import { FormsModule } from '@angular/forms';

import { AppRoutingModule } from './app-routing.module';

import { AppComponent } from './app.component';
import { MainPageComponent } from './pages/main-page/main-page.component';
import { ContainerImageComponent } from './components/container-image/container-image.component';
import { ContainerTextComponent } from './components/container-text/container-text.component';

import { GenericService } from './services/generic.service';
import { FactService } from './services/fact.service';

@NgModule({
  declarations: [
    AppComponent,
    MainPageComponent,
    ContainerImageComponent,
    ContainerTextComponent,
  ],
  imports: [BrowserModule, AppRoutingModule, HttpClientModule, FormsModule],
  providers: [GenericService, FactService],
  bootstrap: [AppComponent],
})
export class AppModule {}
