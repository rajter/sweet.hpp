// Copyright: Robert "burner" Schadek rburners@gmail.com License: GPL 3 or higher
#include "amber.hpp"

static void createIndent(std::ostream& out, const size_t indent) {
	for(size_t i = 0; i < indent; ++i) {
		out<<"\\t";	
	}
}

static std::ostream& Beg(std::ostream& out) {
	out<<"\tout<<\"\\n";
	return out;
}

static std::ostream& End(std::ostream& out) {
	out<<"\";\n";
	return out;
}

static void formatNormalLine(std::ostream& out, const std::string& str, 
		const size_t indent) 
{
	size_t i = 0;
	const size_t ss = str.size();

	while(true) {
		auto b = str.find("&{{", i);

		b = (b == std::string::npos ? ss : b);

		if(b > i) {
			auto iter = str.begin() + i;
			auto iterB = str.begin() + b;
			while(iter != iterB && std::isspace(*iter)) {
				++iter;
			}

			if(iter == iterB) {
				break;
			}
			Beg(out);
			createIndent(out, indent);
			assert(!std::isspace(*iter));
			std::copy(iter, iterB, 
				std::ostream_iterator<char>(out));
			End(out);
		}

		if(b == ss) {
			break;
		} else {
			auto e = str.find("}}&", b);
			assert(e != std::string::npos);

			auto iter = str.begin() + b + 3;
			auto iterB = str.begin() + e;
			out<<"\tout<<";
			out<<"\"";
			createIndent(out, indent);
			out<<"\"<<";
			out<<"(";
			//std::copy(str.begin() + b + 3, str.begin() + e, 
			std::copy(iter, iterB, 
				std::ostream_iterator<char>(out));
			out<<");\n";

			i = e + 3u;
		}
	}
}

void Node::gen(std::ostream& out, const size_t indent) { 
	Beg(out);
	createIndent(out, indent);
	out<<'<'<<this->type;
	if(!this->idLit.empty()) {
		out<<" id=\\\""<<this->idLit<<"\\\"";
	}
	if(!this->classLit.empty()) {
		out<<" class=\\\""<<this->classLit<<"\\\"";
	}
	if(!this->attributes.empty()) {
		// newlines kill c++ string therefore we change thoughs into spaces
		out<<' ';
		for(auto it = this->attributes.begin(); it != this->attributes.end(); 
				++it)
		{
			if(*it == '\n') {
				out<<' ';
				for(; it != this->attributes.end() && std::isspace(*it); ++it) {}
				--it;
			} else {
				out<<*it;
			}
		}
	}

	if(this->openClose) {
		out<<"/>";
		End(out);
		return;
	}

	out<<">";
   	End(out);
	for(auto& it : children) {
		it->gen(out, indent+1);
	}

	Beg(out);
	createIndent(out, indent);
	out<<"</"<<this->type<<">";
	End(out);
}

void CNode::gen(std::ostream& out, const size_t) { 
	out<<this->program<<'\n';
}

void TNode::gen(std::ostream& out, const size_t indent) { 
	Pos pos;
	auto be = this->line.begin();
	auto en = this->line.end();

	eatWhitespace(be, en, pos);

	if(!test(be, en, "&{{") && test(be, en, '&')) {
		++be;
		eatWhitespace(be, en, pos);
		//createIndent(out, indent);
		out<<'\t';
		std::copy(be, en, std::ostream_iterator<char>(out));
	} else {
		formatNormalLine(out, this->line, indent);
	}
	out<<'\n';
}
