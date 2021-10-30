package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strings"

	json "encoding/json"

	easyjson "github.com/mailru/easyjson"
	jlexer "github.com/mailru/easyjson/jlexer"
	jwriter "github.com/mailru/easyjson/jwriter"
	// "log"
)

// Structure to contain everything about user
type userInfo struct {
	Browsers []string `json:"browsers,"`
	Company  string   `json:"-"`
	Country  string   `json:"-"`
	Email    string   `json:"email,"`
	Job      string   `json:"-"`
	Name     string   `json:"name,"`
	Phone    string   `json:"-"`
}

// Easyjson generated methods

// suppress unused package warning
var (
	_ *json.RawMessage
	_ *jlexer.Lexer
	_ *jwriter.Writer
	_ easyjson.Marshaler
)

func easyjson42239ddeDecodeHw3BenchJson(in *jlexer.Lexer, out *userInfo) {
	isTopLevel := in.IsStart()
	if in.IsNull() {
		if isTopLevel {
			in.Consumed()
		}
		in.Skip()
		return
	}
	in.Delim('{')
	for !in.IsDelim('}') {
		key := in.UnsafeFieldName(false)
		in.WantColon()
		if in.IsNull() {
			in.Skip()
			in.WantComma()
			continue
		}
		switch key {
		case "browsers":
			if in.IsNull() {
				in.Skip()
				out.Browsers = nil
			} else {
				in.Delim('[')
				if out.Browsers == nil {
					if !in.IsDelim(']') {
						out.Browsers = make([]string, 0, 4)
					} else {
						out.Browsers = []string{}
					}
				} else {
					out.Browsers = (out.Browsers)[:0]
				}
				for !in.IsDelim(']') {
					var v1 string
					v1 = string(in.String())
					out.Browsers = append(out.Browsers, v1)
					in.WantComma()
				}
				in.Delim(']')
			}
		case "email":
			out.Email = string(in.String())
		case "name":
			out.Name = string(in.String())
		default:
			in.SkipRecursive()
		}
		in.WantComma()
	}
	in.Delim('}')
	if isTopLevel {
		in.Consumed()
	}
}
func easyjson42239ddeEncodeHw3BenchJson(out *jwriter.Writer, in userInfo) {
	out.RawByte('{')
	first := true
	_ = first
	{
		const prefix string = ",\"browsers\":"
		out.RawString(prefix[1:])
		if in.Browsers == nil && (out.Flags&jwriter.NilSliceAsEmpty) == 0 {
			out.RawString("null")
		} else {
			out.RawByte('[')
			for v2, v3 := range in.Browsers {
				if v2 > 0 {
					out.RawByte(',')
				}
				out.String(string(v3))
			}
			out.RawByte(']')
		}
	}
	{
		const prefix string = ",\"email\":"
		out.RawString(prefix)
		out.String(string(in.Email))
	}
	{
		const prefix string = ",\"name\":"
		out.RawString(prefix)
		out.String(string(in.Name))
	}
	out.RawByte('}')
}

// MarshalJSON supports json.Marshaler interface
func (v userInfo) MarshalJSON() ([]byte, error) {
	w := jwriter.Writer{}
	easyjson42239ddeEncodeHw3BenchJson(&w, v)
	return w.Buffer.BuildBytes(), w.Error
}

// MarshalEasyJSON supports easyjson.Marshaler interface
func (v userInfo) MarshalEasyJSON(w *jwriter.Writer) {
	easyjson42239ddeEncodeHw3BenchJson(w, v)
}

// UnmarshalJSON supports json.Unmarshaler interface
func (v *userInfo) UnmarshalJSON(data []byte) error {
	r := jlexer.Lexer{Data: data}
	easyjson42239ddeDecodeHw3BenchJson(&r, v)
	return r.Error()
}

// UnmarshalEasyJSON supports easyjson.Unmarshaler interface
func (v *userInfo) UnmarshalEasyJSON(l *jlexer.Lexer) {
	easyjson42239ddeDecodeHw3BenchJson(l, v)
}

// End of generated methods

func FastSearch(out io.Writer) {
	file, err := os.Open(filePath)
	if err != nil {
		panic(err)
	}

	fileScanner := bufio.NewScanner(file)
	seenBrowsers := map[string]struct{}{}
	uniqueBrowsers := 0

	// Read each user's info
	counter := -1
	userInfo := userInfo{}

	// Pre-output for not collecting the whole string
	fmt.Fprintln(out, "found users:")
	for fileScanner.Scan() {
		counter++

		err := easyjson.Unmarshal(fileScanner.Bytes(), &userInfo)
		if err != nil {
			panic(err)
		}

		isAndroid := false
		isMSIE := false
		browsers := userInfo.Browsers

		for _, browser := range browsers {
			if ok := strings.Contains(browser, "Android"); ok {
				isAndroid = true
				if _, ok := seenBrowsers[browser]; !ok {
					seenBrowsers[browser] = struct{}{}
					uniqueBrowsers++
				}
			}

			if ok := strings.Contains(browser, "MSIE"); ok {
				isMSIE = true
				if _, ok := seenBrowsers[browser]; !ok {
					seenBrowsers[browser] = struct{}{}
					uniqueBrowsers++
				}
			}
		}

		if !(isAndroid && isMSIE) {
			continue
		}

		email := strings.Replace(userInfo.Email, "@", " [at] ", 1)
		fmt.Fprintf(out, "[%d] %s <%s>\n", counter, userInfo.Name, email)
	}

	fmt.Fprintln(out, "\nTotal unique browsers", uniqueBrowsers)
}
